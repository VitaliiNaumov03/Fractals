## About
This program visualizes Sierpiński&#39;s triangle and Barnsley&#39;s Fern by calculating and drawing each point in real-time, so you can see how these fractals appear out of nothing before your eyes.
## Results
The more time you give it, the more detailed it will be.
### Sierpiński&#39;s triangle
![1](https://github.com/user-attachments/assets/1f8f9cc8-14e5-4d9e-9cdb-7b2feea3cd59)
### Barnsley&#39;s Fern
![2](https://github.com/user-attachments/assets/170447a9-59ef-4e9b-89de-c56bc0ef453a)
## Compiling
To build with MinGW (GCC), use this commands:
```
cmake -B build -S . -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
