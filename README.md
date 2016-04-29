## spl

my handy c++ toolkit

### setup on ubuntu
```
# 0. 安装 cmake gtest
sudo apt-get install cmake libgtest-dev
# 1. 进入 gtest 源码目录
cd /usr/src/gtest
# 2. 创建编译目录
sudo cmake -E make_directory build
# 3. 生成 Makefile
sudo cmake -E chdir build cmake ..
# 4. 编译
sudo cmake --build build
# 5. 拷贝库文件
sudo cp build/libgtest* /usr/local/lib/
# 6. 删除无用的目录
sudo rm -rf build
```
