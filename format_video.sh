#!/bin/bash

# sudo apt-get install xdotool imagemagick xwininfo ffmpeg
set -x

# Check if ffmpeg is installed
if ! command -v ffmpeg &> /dev/null
then
    echo "ffmpeg could not be found, please install it first."
    exit
fi

rm -rf resized_video
mkdir -p resized_video
# First, let's resize all the videos
for file in video/*.mp4
do
    filename_with_extension=$(basename "$file")
    filename=${filename_with_extension%.*}
    ffmpeg -nostdin -i "$file" -vf scale=400:-1 -preset slow -crf 18 "resized_video/${filename}_resized.mp4"
done

rm output.mp4

# Now, we create an array that holds all our video inputs
videoInputs=()
for file in resized_video/*_resized.mp4
do
    videoInputs+=("-i $file")
done

# Check to make sure we have a multiple of 5 videos
if (( ${#videoInputs[@]} % 5 != 0 )); then
    echo "Error: The number of videos must be a multiple of 5."
    exit 1
fi

# Now, let's create the filter for stacking 5 videos in a row
filter=""
vstack_inputs=""
for (( i=0; i<${#videoInputs[@]}; i+=5 ))
do
    rowFilter="[${i}:v][$((i+1)):v][$((i+2)):v][$((i+3)):v][$((i+4)):v]hstack=inputs=5[row$((i/5))];"
    filter+=$rowFilter
    vstack_inputs+="[row$((i/5))]"
done
# filter=${filter::-1}  # remove the last semicolon
# filter+="vstack=inputs=${#videoInputs[@]/5}"
filter+=$vstack_inputs
filter+="vstack=inputs=$(( ${#videoInputs[@]} / 5 ))"


# Finally, let's create our final video
ffmpeg -nostdin ${videoInputs[@]} -filter_complex "$filter" output.mp4
