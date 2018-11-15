loader_file=$1
kernel_file=$2

# By default
img_file="os.img"

# Constants
BLOCK_SIZE=512

# The loader sits on the segment 0
KERNEL_SEGMENT=1

# The total number of segments to have
TOTAL_SEGMENTS=2880

# If 3 arguments were passed, then get the img_file from $3
if [ $# -eq 3 ]
then
	img_file="$3"
fi

# 
# The core of the script
# 

# Write the loader
dd if=$loader_file of=$img_file bs=$BLOCK_SIZE seek=0

# Write the kernel
dd if=$kernel_file of=$img_file bs=$BLOCK_SIZE seek=$KERNEL_SEGMENT

# Do some magic stuff
dd if=/dev/zero of=$img_file bs=$BLOCK_SIZE count=0 seek=$TOTAL_SEGMENTS
