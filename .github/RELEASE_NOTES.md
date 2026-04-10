🎉 **What's new in this version:**

🚀 **Smart multithreading**

- No more hard limits of 4 threads by default. Now the program **automatically analyzes your hardware** and runs in the optimal mode of `Maximum cores - 1`. This gives the maximum speed of image processing, keeping the system responsive!

🪟 **Improvements for Windows users**

- Fixed the problem of the window closing instantly. Now, if you just double-click on `ImageProcessor.exe`, the program will politely show a detailed help with a list of all filters and wait for you to press `Enter`

🧪 **Increased reliability** Implemented strict validation of the thread limit
(now the program will not crash when you enter inadequate values)

- Added a successful stress test of the **Gaussian Blur** algorithm (heavy blur with a radius of 100). The program can handle extreme loads consistently.
