# Cover

This is inspired by [this video](youtube.com/watch?v=OiuZe3rmxqM) and [this video](youtube.com/quXacw3z0dY?si=U0TTmZ7AA1_iklG3), though the code is my own. This is my attempt to create a cover generating software from scratch. 

I have like no idea what the common algorithm is, so I'm going into this completely blind! I wonder how I'll do on this, especially considering I want to finish this in one day. I'll probably go completely without sleep or something. 

# Dependencies

***IMPORTANT***: this project uses `brew` and `ffmpeg`. First install brew, then do `brew install ffmpeg`. You may need "minor" tweaks and fixes (sorry) to get this to work, but I'm sure that it won't require changing of code, only the Makefile.

## Implementation

From what I've gathered, I need to decode the mp3 file into pulse-code modulation (pcm) using this library called `ffmpeg`, then insert it into an algorithm called a Fast Fourier Transform (fft). This will extract its frequency data. 

Then, this is mostly my own algorithm synopsys. I'll have a constant dictating how many runs of the algorithm I use and the minimum length of each sample used. For each run, I will go through the entire frequency data of the file, using a sliding window with minimum length specified in the aforementioned constant. Then, I will find the best match in the cover, and subtract the frequency data from each other. I will keep the subtractions so that each subsequent run will progressively make the sounds closer.
