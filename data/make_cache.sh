# do not forget to change USERNAME
#in linux, symlink exiftool to ./exiftool.exe
SEARCH_DIR='/cygdrive/f'
USERNAME='lubka_fotky'
CACHE_DIR='cache'
CACHE_DATA='cache_data'

SKIP_HASH_FILE='skip_hash'

GEOMETRY="200x150"
CNT=0
CNT_CACHED=0
CNT_SKIPPED=0

find  "$SEARCH_DIR" -iname \*.jp\*g | while read FILE
do
  let CNT=$CNT+1
  echo "processing $FILE ..."

  if grep "$FILE" $SKIP_HASH_FILE > /dev/null; then
    echo "Already in cache"
    let CNT_SKIPPED=$CNT_SKIPPED+1
  else
      # linux:
      # WINFILE="$FILE"
      
      # windows:
      WINFILE=`cygpath --windows "$FILE"`
      HASH=`shasum "$FILE" | sed 's/ .*//'`
      
      if grep $HASH $SKIP_HASH_FILE > /dev/null; then
        echo "file $FILE already in cache";
        let CNT_SKIPPED=$CNT_SKIPPED+1
      else
        echo "file $FILE, hash $HASH"
        IMAGE_SIZE=`./exiftool.exe -imagesize "$WINFILE" | sed 's/.*: //'`
        if echo "$IMAGE_SIZE" | python check_size.py; then
            echo "caching file..."
            convert "$FILE" -geometry "$GEOMETRY!" -quality 95 tmp.jpg
            HASH2=`shasum tmp.jpg | sed 's/ .*//'`
            HASH_DIR=`echo $HASH2 | sed 's/\(..\).*/\1/'`
            mkdir -p "$CACHE_DIR/$HASH_DIR"
            mv tmp.jpg "$CACHE_DIR/$HASH_DIR/$HASH2.jpg"
            echo "$HASH2|$HASH|$USERNAME|$FILE" >> $CACHE_DATA
            echo "file done"
            let CNT_CACHED=$CNT_CACHED+1

        else
            echo "Image have wrong size/ratio $IMAGE_SIZE"
        fi

        echo "$HASH|$USERNAME|$FILE" >> $SKIP_HASH_FILE 
      fi
  fi


  echo "" 
  echo "-----------($CNT_CACHED cached, $CNT_SKIPPED skipped of $CNT total images)"
  echo ""

done

echo "Done"




