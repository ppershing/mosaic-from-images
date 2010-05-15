# in linux, symlink exiftool to ./exiftool.exe
# or change
WIN=1
if [ $WIN -eq 1 ]
then
    EXIFTOOL="./exiftool.exe"
else
    EXIFTOOL="exiftool"
fi

# common file names
CACHE_DIR='cache'
CACHE_DATA='cache_data'
SKIP_HASH_FILE='skip_hash'
# how big the thumbnails should be
# this is good default for caching
GEOMETRY="200x150"

if [ $# -ne 2 ]
then
    echo "Usage: $0 user_comment search_dir"
    echo "(user_comment is meant to help if user wants to find"
    echo " from where some photo comes)";
    echo "user_comment and search_dir(subdirs included)"
    echo "mustn't contain character '|'";
    exit 1
fi

USERCOMMENT="$1"
SEARCH_DIR="$2"

touch $CACHE_DATA
touch $SKIP_HASH_FILE
mkdir $CACHE_DIR

# internal counts
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
    if [ $WIN -eq 1 ]
    then
        WINFILE=`cygpath --windows "$FILE"`
    else
        WINFILE="$FILE"
    fi
      
    HASH=`shasum "$FILE" | sed 's/ .*//'`
      
    if grep $HASH $SKIP_HASH_FILE > /dev/null; then
        echo "file $FILE already in cache";
        let CNT_SKIPPED=$CNT_SKIPPED+1
    else
        echo "file $FILE, hash $HASH"
        IMAGE_SIZE=`$EXIFTOOL -imagesize "$WINFILE" | sed 's/.*: //'`
        EXIFRET=$?
        if [ $EXIFRET -ne 0 ]
        then
            echo "!!!!!! EXIFTOOL ERROR";
        fi

        if echo "$IMAGE_SIZE" | python check_size.py; then
            echo "caching file..."
            convert "$FILE" -geometry "$GEOMETRY!" -quality 95 tmp.jpg
            CONVERTRET=$?
            if [ $CONVERTRET -ne 0 ]
            then
                echo "!!!!!! CONVERT ERROR"
            fi
            HASH2=`shasum tmp.jpg | sed 's/ .*//'`
            HASH_DIR=`echo $HASH2 | sed 's/\(..\).*/\1/'`
            mkdir -p "$CACHE_DIR/$HASH_DIR"
            mv tmp.jpg "$CACHE_DIR/$HASH_DIR/$HASH2.jpg"
            echo "$HASH2|$HASH|$USERCOMMENT|$FILE" >> $CACHE_DATA
            echo "file done"
            let CNT_CACHED=$CNT_CACHED+1

        else
            echo "Image have wrong size/ratio $IMAGE_SIZE"
        fi

        echo "$HASH|$USERCOMMENT|$FILE" >> $SKIP_HASH_FILE 
      fi
  fi


  echo "" 
  echo "-----------($CNT_CACHED cached, $CNT_SKIPPED skipped of $CNT total images)"
  echo ""

done

echo "Done"




