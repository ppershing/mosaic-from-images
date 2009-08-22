import sys;

MIN_X = 400
RATIO = 4.0/3.0
RATIO_DIFF = 1.10
MAX_RATIO = RATIO * RATIO_DIFF
MIN_RATIO = RATIO / RATIO_DIFF

line=sys.stdin.readline().split("x");
x = int(line[0])
y = int(line[1])


if (x < MIN_X):
    print "Wrong size, too small"
    exit(1)

x = float(x)
y = float(y)

ratio = x/y

print "Ratio is %.3f" % (ratio)

if (ratio > MAX_RATIO) or (ratio < MIN_RATIO):
    print "Wrong ratio"
    exit(1)


exit(0)

