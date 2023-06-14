#!/bin/bash

# sudo apt-get install xdotool imagemagick xwininfo ffmpeg
set -x

# Check if ffmpeg is installed
if ! command -v ffmpeg &> /dev/null
then
    echo "ffmpeg could not be found, please install it first."
    exit
fi

# Check if xwininfo is installed
if ! command -v xwininfo &> /dev/null
then
    echo "xwininfo could not be found, please install it first."
    exit
fi


DURATION=5

# Create the output directory for screenshots
rm -rf screenshots
mkdir -p screenshots
rm -rf video
mkdir -p video

# Iterate through all executables in the build/ directory
find build/ -maxdepth 1 -type f -executable | while read -r exe_path; do
  # Get the base name of the executable
  exe_name=$(basename "$exe_path")

  # Run the executable
  $exe_path &
  exe_pid=$!

  # Wait for the window to appear
  sleep 1

  # Get the window ID of the running executable
  window_id=$(xdotool search --pid "$exe_pid" | head -n 1)

  # Get window information
  INFO=$(xwininfo -id $window_id)
  # Extract window position and size information
  X=$(echo $INFO | grep -oEe 'Absolute upper-left X:.*' | awk '{print $4}')
  Y=$(echo $INFO | grep -oEe 'Absolute upper-left Y:.*' | awk '{print $4}')
  WIDTH=$(echo $INFO | grep -oEe 'Width:.*' | awk '{print $2}')
  HEIGHT=$(echo $INFO | grep -oEe 'Height:.*' | awk '{print $2}')

  # Record the video
  # Add -nostdin to keep the loop working properly
  ffmpeg -nostdin -video_size ${WIDTH}x${HEIGHT} -framerate 60 -f x11grab -i :1+${X},${Y} -t $DURATION "video/$exe_name.mp4"
  
  # Take a screenshot of the window and save it in the screenshots directory
  # import -window "$window_id" "screenshots/$exe_name.png"

  # Kill the running executable and its parent xvfb-run process
  kill "$exe_pid"
  pkill -P "$exe_pid"

  # Wait a little before processing the next executable
  sleep 1
  # break
done

# Merge all screenshots into a single image using ImageMagick
# convert -append screenshots/*.png final_screenshot.png
