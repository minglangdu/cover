# Cover

This is my attempt to create a cover generating software from scratch. I hope you enjoy!

# Dependencies

***IMPORTANT***: this project uses `brew` and `ffmpeg`. First install brew, then do `brew install ffmpeg`. You may need "minor" tweaks and fixes (sorry) to get this to work, but I'm sure that it won't require changing of code, only the Makefile.

## Implementation

From what I've gathered, I need to decode the mp3 file into pulse-code modulation (pcm) using this library called `ffmpeg`, then read it into an array of `short`s. Since my current usage of `ffmpeg` converts the file into a 16-bit pcm, I'll use the 2-byte (or 16-bit) `short`s in a vector representing the sound wave. This will extract its frequency data. 

Then, this is mostly my own algorithm synopsys. I'll have a constant dictating how many runs of the algorithm I use and the minimum length of each sample used. For each run, I will go through the entire frequency data of the file, using a sliding window with minimum length specified in the aforementioned constant. Then, I will find the best match in the cover, and subtract the frequency data from each other. I will keep the subtractions so that each subsequent run will progressively make the sounds closer. All mappings of one mp3 to another will be stored, mostly for debug purposes but also if you want to enhance the editing manually.

For the best match, I will be testing out many different evaluations. Firstly, I will try the crudest and easiest implementation, which is just using how close the sums of each frequency in the sample size are. I'll create a vector of size (N - MIN_SIZE + 1), storing, for each `v[i]`, the sum of the elements in the array from `a[i]` to `a[i + MIN_SIZE - 1]`. Then, I will place these sums as the first parts of pairs, with the second part storing the original index. I will sort the vector and use `lower_bound` and some `if` statements to find the closest sum to the current section of the covered song. 

Another implementation could also be to minimize the sum of the squares of the differences between each frequency in the covered and used vectors. This will place more emphasis on making samples actually fit, but it would also be a nightmare to optimize. Definitely in the longer term for now. 