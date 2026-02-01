# WasmRay: High-Performance Real-Time Ray Tracer

**WasmRay** is a real-time ray tracing engine built in **C++** and compiled to **WebAssembly (Wasm)**. It renders interactive 3D graphics directly into an HTML5 Canvas element.

By leveraging the raw performance of C++ and the portability of Wasm, this project achieves near-native frame rates in the browser, demonstrating efficient shared memory management between the JavaScript frontend and the C++ backend.

*(Replace this line with an actual screenshot of your running project)*

## 🚀 Key Features

* **Real-Time Rendering:** Renders 3D spheres with mathematical precision at ~60 FPS.
* **Zero-Copy Memory Access:** JavaScript reads pixel data directly from the C++ heap via `Uint8ClampedArray` views, eliminating expensive serialization overhead.
* **Vector Math Engine:** Custom C++ library for vector operations (dot products, normalization, intersections).
* **Dynamic Lighting:** Real-time calculation of Lambertian shading with a moving light source.
* **No External Graphics Libraries:** No WebGL or OpenGL—pure mathematical ray casting written from scratch.

## 🛠️ Tech Stack

* **Core Logic:** C++17 (Vector math, ray intersection logic).
* **Compiler:** Emscripten (emsdk) for C++ to Wasm compilation.
* **Frontend:** HTML5 Canvas, JavaScript (for rendering the WASM memory buffer).
* **Build Tool:** Bash / Emscripten CLI.

## 🧠 Technical Highlights (Under the Hood)

This project demonstrates several systems engineering concepts:

### 1. Direct Memory Manipulation

Instead of passing arrays back and forth between JS and C++ (which causes garbage collection pauses), `WasmRay` allocates a single linear memory buffer in C++. JavaScript wraps this pointer in a `Uint8ClampedArray` to paint the canvas.

```javascript
// JavaScript accesses C++ memory directly
const bufferView = new Uint8ClampedArray(
    Module.HEAPU8.buffer, // The Wasm Heap
    bufferPtr,            // Pointer returned by C++
    width * height * 4    // Size
);

```

### 2. Ray-Sphere Intersection

The engine solves the quadratic equation for every pixel to determine intersections:


### 3. Optimization

Compiled with the `-O3` Emscripten flag to enable aggressive Loop Vectorization and function inlining, ensuring the C++ code runs at near-native speeds within the browser VM.

## 💻 Getting Started

### Prerequisites

* **Emscripten SDK (emsdk):** [Installation Guide](https://emscripten.org/docs/getting_started/downloads.html)
* **Python 3** (or any local web server).

### Installation & Build

1. **Clone the repository:**
```bash
git clone https://github.com/yourusername/WasmRay.git
cd WasmRay

```


2. **Compile the C++ code:**
Run the following command to generate the `raytracer.js` glue code and `raytracer.wasm` binary.
```bash
emcc main.cpp -o raytracer.js \
    -s EXPORTED_FUNCTIONS="['_render', '_getBuffer']" \
    -s EXPORTED_RUNTIME_METHODS="['ccall', 'cwrap']" \
    -O3 \
    -s ALLOW_MEMORY_GROWTH=1

```


3. **Run the Local Server:**
Because of CORS policies regarding WebAssembly, you must serve the files via a local server.
```bash
python3 -m http.server

```


4. **View the Project:**
Open your browser to `http://localhost:8000`.

## 🔮 Future Improvements

* Implementation of **Shadows** (checking for object occlusion between hit point and light).
* Adding **Recursive Reflections** for mirror-like surfaces.
* Multithreading using **Web Workers** and `pthread` support in Emscripten.
