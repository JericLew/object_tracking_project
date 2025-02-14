# Object Tracking OpenCV

Small OpenCV project for object detection and tracking.

## Environment

1. OS: Ubuntu 20.04
2. GPU: NVIDIA GeForce RTX 3050 Ti Laptop
3. CPU: AMD Ryzen 7 5800H with Radeon Graphics
4. CUDA: CUDA 11.4
5. CUDNN: CUDNN 8.9.2.26-1+cuda11.8
6. OpenCV: OpenCV 4.5.4
7. Python: python 3.8
8. pyTorch: torch 1.11.0+cu113
9. torchvision: torchvision 0.12.0+cu113
10. YOLOv5: YOLOv5 Release v7.0 (abit more updated)

## Setup

### CUDA

CUDA is optional but recommended to speed-up inference and training speeds with OpenCV and pyTorch

[CUDA Install Instructions](https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html#package-manager-installation)

[CUDA Download](https://developer.nvidia.com/cuda-toolkit-archive)

### CUDNN

CUDNN is optional but is required for OpenCV DNN acceleration

[CUDNN Install Instructions](https://docs.nvidia.com/deeplearning/cudnn/install-guide/index.html)

[CUDNN Download](https://developer.nvidia.com/rdp/cudnn-download)

### OpenCV

OpenCV and OpenCV contrib is required and has to be build from source if using CUDA and CUDNN acceleration.


1. Enter working directory
   ```sh
   cd /path/to/tracking/ws
   ```

2. Run setup.sh script
   ```sh
   sudo chmod +x setup.sh
   ./setup.sh
   ```

3. Install OpenCV and OpenCV contrib without CUDA and CUDNN
   ```sh
   sudo apt-get update
   sudo apt-get install libopencv-dev=4.5.4 libopencv-python=4.5.4
   ```

   OR

   Install OpenCV and OpenCV contrib with CUDA and CUDNN
   Please refer to documentation from [OpenCV](https://docs.opencv.org/4.5.4/d7/d9f/tutorial_linux_install.html) for install

   Use the below CMake script:
   Please do change what ever applicable i.e. 
   - `-DCMAKE_INSTALL_PREFIX`
   - `-DCUDA_TOOLKIT_ROOT_DIR`
   - `-DOPENCV_EXTRA_MODULES_PATH`

   ```sh
   cmake -DWITH_OPENGL=ON -DENABLE_PRECOMPILED_HEADERS=OFF -DBUILD_opencv_cudacodec=OFF -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=~/opencv-4.5.4-linux -DWITH_TBB=ON -DBUILD_EXAMPLES=OFF -DBUILD_opencv_world=OFF -DBUILD_opencv_gapi=ON -DBUILD_opencv_wechat_qrcode=OFF -DWITH_QT=ON -DWITH_OPENGL=ON -DWITH_GTK=ON -DWITH_GTK3=ON -DWITH_GTK_2_X=OFF -DWITH_VTK=OFF -DWITH_CUDA=ON -DWITH_CUDNN=ON -DOPENCV_DNN_CUDA=ON -DWITH_CUBLAS=ON -DCUDA_TOOLKIT_ROOT_DIR=/usr/local/cuda-11.4 -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules ..
   ```

   Open a terminal and export library path as per `-DCMAKE_INSTALL_PREFIX`
   ```sh
   echo 'export LD_LIBRARY_PATH=~/opencv-4.5.4-linux/local/lib:$LD_LIBRARY_PATH' >> ~/.bashrc
   echo 'export PYTHONPATH=~/opencv-4.5.4-linux/local/lib/python3.8/dist-packages/:$PYTHONPATH' >> ~/.bashrc
   source ~/.bashrc
   ```
   
### virtualenv
Setup python virtual environment to install pyTorch and YOLO python packages

```sh
sudo apt-get install virtualenv
cd ~/
virtualenv env_track --system-site-packages
echo 'export LD_LIBRARY_PATH=~/opencv-4.5.4-linux/local/lib:$LD_LIBRARY_PATH' >> ~/env_track/bin/activate
echo 'export PYTHONPATH=~/opencv-4.5.4-linux/local/lib/python3.8/dist-packages/:$PYTHONPATH' >> ~/env_track/bin/activate
source ~/env_track/bin/activate
```

### pyTorch

Install pyTorch and its compenent compatible with your CUDA version.
```sh
pip install torch==1.11.0+cu113 torchvision==0.12.0+cu113 torchaudio==0.11.0 --extra-index-url https://download.pytorch.org/whl/cu113
```

### YOLOv5

Install YOLOv5 using instructions from their [documentation](https://docs.ultralytics.com/yolov5/quickstart_tutorial/)

#### Inference
```sh
python detect.py --weights ~/yolov5/run/train/5_epoch_all/weights/best.pt --source ~/tracking_ws/videos/video1.avi --view-img
```

#### Training
Warning: using --batch-size 12 or lower for training
```sh
python train.py --img 640 --epochs 300 --data merge_class_random_split.yaml --weights yolov5s.pt --batch-size 64 --device 0 --optimizer AdamW --patience 50 --save-period 50
```

#### Validation
```sh
python val.py --weights /path/to/model.pt --data ./data/merge_class_random_split.yaml --batch-size 64 --device 0 --verbose
```

#### Export
Warning: using --opset 11 for exporting to ONNX
```sh
python3 export.py --weights best.pt --include onnx --device 0 --opset 11
```

## Usage

### CPP

Before usage of CPP code, open CMakeLists.txt and change directory of OpenCV 4.5.4 install

1. Enter Detection/Tracking directory
   ```sh
   cd ~/tracking_ws/cpp/src/Detection
   ```

2. Create and enter build directory
   ```sh
   mkdir build && cd build
   ```

3. Initialse CMake from CMakeLists.txt
   ```sh
   cmake ..
   ```

4. Build cpp source code
   ```sh
   make
   ```

#### Detection

Run Detection executable
Arguements are /path/to/tracking/ws and /path/to/video/input

```sh
./Detection ~/tracking_ws/ ~/tracking_ws/videos/video1.avi
```

#### Tracking

Run Tracking executable
Arguements are /path/to/tracking/ws, /path/to/video/input and tracker name (MOSSE, CSRT etc.)

```sh
./Tracking ~/tracking_ws/ ~/tracking_ws/videos/video1.avi MOSSE
```

### Python

Run python script using python 3.8.

```sh
python3 ~/tracking_ws/python/src/detection.py ~/tracking_ws/ ~/tracking_ws/videos/video1.avi
```

```sh
python3 ~/tracking_ws/python/src/tracking.py ~/tracking_ws/ ~/tracking_ws/videos/video1.avi
```

### Models

There might be some issues when using the .onnx models, if so use YOLOv5 commands as above to export .pt model to .onnx for OpenCV DNN.

Below are the descriptions of the different trained models:

300_0.7_image-weights: (image-weight sampling)
python train.py --img 640 --epochs 300 --data merge_class_random_split.yaml --weights yolov5s.pt --batch-size 64 --device 0 --optimizer AdamW --patience 50 --save-period 50 --image-weights

exp: (weighted loss)
python train_weighted.py --img 640 --epochs 300 --data merge_class_random_split.yaml --weights yolov5s.pt --batch-size 64 --device 0 --optimizer AdamW --patience 50 --save-period 50

exp2: (image-weight sampling + augmentation)
python train.py --img 640 --epochs 300 --data merge_class_random_split.yaml --hyp ./data/hyps/hyp.scratch-low-edited.yaml --weights yolov5s.pt --batch-size 64 --device 0 --optimizer AdamW --patience 50 --save-period 50 --image-weights

exp3: (image-weight sampling + augmentation + scratch)
python train.py --img 640 --epochs 300 --data merge_class_random_split.yaml --hyp ./data/hyps/hyp.scratch-low-edited.yaml --weights '' --cfg yolov5s.yaml --batch-size 64 --device 0 --optimizer AdamW --patience 50 --save-period 50 --image-weights

exp4: (image-weight sampling + augmentation + scratch)
python train.py --img 640 --epochs 1000 --data merge_class_random_split.yaml --hyp ./data/hyps/hyp.scratch-low-edited.yaml --weights '' --cfg yolov5s.yaml --batch-size 64 --device 0 --optimizer AdamW --patience 50 --save-period 50 --image-weights

exp5: (weighted loss + augmentation + scratch)
python train_weighted.py --img 640 --epochs 1000 --data merge_class_random_split.yaml   --hyp ./data/hyps/hyp.scratch-low-edited.yaml --weights '' --cfg yolov5s.yaml --batch-size 64 --device 0 --optimizer AdamW --patience 150 --save-period 50

## Useful Links

### Xavier NX

#### Setup

- https://developer.nvidia.com/embedded/learn/get-started-jetson-xavier-nx-devkit
- https://docs.nvidia.com/deeplearning/frameworks/install-pytorch-jetson-platform/index.html (pyTorch Install)
- https://developer.nvidia.com/embedded/downloads (Different pyTorch Versions)
- https://catalog.ngc.nvidia.com/orgs/nvidia/containers/l4t-pytorch (Using Docker Image)
- https://nvidia-ai-iot.github.io/jetson-min-disk/ (Minimise Disk Space)

#### TensorRT

I recommend to read through all of them to understand the options available as TensorRT can be used differently.

- https://docs.nvidia.com/deeplearning/tensorrt/install-guide/index.html#gettingstarted (TensorRT Install)
- https://roboticsknowledgebase.com/wiki/machine-learning/yolov5-tensorrt/ (Guide on converting to TensorRT using CLI with Jetson + ROS2)
- https://learnopencv.com/how-to-convert-a-model-from-pytorch-to-tensorrt-and-speed-up-inference/ (Guide on converting to TensorRT with python)
- https://learnopencv.com/how-to-run-inference-using-tensorrt-c-api/ (Using TensorRT with C++)
- https://www.seeedstudio.com/blog/2022/08/23/faster-inference-with-tensorrt-on-nvidia-jetson-run-yolov5-at-27-fps-on-jetson-nano/ (Guide on YOLOv5 with Jetson, uses NVIDIA Deepstream)
- https://github.com/wang-xinyu/tensorrtx (Open Source TensorRT convertor and inference engine)
- https://github.com/NVIDIA-AI-IOT/torch2trt (NVIDIA's TensorRT convertor)

### Object Tracking

Here are some Object Tracker repos that are interesting

- https://github.com/abewley/sort
- https://github.com/nwojke/deep_sort/tree/master/deep_sort
- https://github.com/Smorodov/Multitarget-tracker
- https://github.com/mcximing/hungarian-algorithm-cpp
