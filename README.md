# Ray Tracer
**A simple ray tracer done for COMP 371 Computer Graphics. The rest of this file is an edit of the original readme file attached to the assignment submission.**

# The Basics
The main.cpp file follows a fairly straightforward logic and is hopefully easy to follow. In order to implement the additional features, but avoid repeating code, I used preprocessor statements wherever possible. These can all be set in libs.h.

The program is written so that reading the main.cpp will give you a good idea of what is happening. The util.cpp was written to help abstract functions that would be called from anywhere. For example, the clip function clamps values and is called by several functions. 

All scenes are rendered with a log file that includes some basic stats about that render.

# Methods & Quirks
Due to a certain amount of obsessing over getting my colours to match the samples given, I ended up learning a lot of interesting things about colour. There was also a certain amount of general implementation quirkiness that came up along the way. This section covers those quirks.

## Colour Merging
In the pursuit of perfect colour, I came across [this article on Medium](https://medium.com/@kevinsimper/how-to-average-rgb-colors-together-6cd3ef1ff1e5). It explains that because of how colour is treated on computers, merging colours is a little more complex than simply taking the average. I implemented this as my merge_colours method. Since I didn’t ever implement just colour average, I do not have any comparisons to show here.

## Gamma Correction
My colours weren’t popping the way I’d like so, after consulting my colleagues, I implemented Gamma Correction. This mainly gave all the colours a somewhat washed out effect. Because this was the only effect, I left the ability in but do not apply it to any of my other renders.

## Sepheres vs Plans in Shadows
After messing with both gamma correction and colour merging, I still had something odd with my shadows. Specifically, the shadows on my spheres had hard lines. See figure 2. After much pestering of professors, peers, and TAs, I thought I’d figured out what the problem was. 

My shadow calculation was Boolean. Meaning that before running any colour calculation I first checked if the object was in shadow. If it was in shadow, I just added the ambient colour times the light colour. If the object wasn’t in shadow, I would run the full Phong colour calculation. The problem is that Phong relies on normals, which are consistent for a plane, but not for a sphere. This meant that if I wanted the soft shadows, I needed to still run Phong even if the sphere was in shadow. This results in a new problem. Namely, it barely has a shadow on it and instead has a sort of washed out effect. This means I’m making a choice between hard sphere shadow or washed out sphere shadow. 

The trouble is that the professor pointed out that this is wrong and after more manipulation of my colour calculation algorithm without any better results I’m left with two possibilities. The first is that there is a problem with my intersection calculation, somewhere along the way it’s eating a few number places which results in the sphere returning in shadow when it shouldn’t. The other possible reason is that my colour calculation is too convoluted. My guess is that it’s the former, but it is too late for someone of my skill level to refactor or test to prove that this is the problem.


# Assignment Bonus Features
Unfortunately, I did not have the time to implement recursive rays. I did manage soft shadows and anti-aliasing. They can be run together. 

## Soft Shadows
These were implemented by creating a child class of light, which treats the light location as the centre of a sphere and takes samples from within the sphere’s radius. The radius and number of samples can be set by the user. See softLight.h for details.

## Anti-Aliasing
This was implemented by using the downsizing method. An initial CImg object would be created which is bigger than the end result (usually every 9x9 pixels would translate to 1). This image would then run through the render before being downsized. Downsizing is done by blending the colour of the surrounding pixels. The blending is done using the merge_colours method mentioned above.

# Acknowledgements
Without the help of my colleagues Mathew Plouffe and Tom Lebreux, this project would have taken a lot longer and come out a lot worse. So, blame me for all its faults and praise them for all its perfections.
