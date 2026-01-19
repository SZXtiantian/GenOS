#  GenOS的功能演示样例 (Sample Project) 

[![Ubuntu](https://img.shields.io/badge/Ubuntu-20.04%20LTS-E95420?style=flat&logo=ubuntu&logoColor=white)](https://ubuntu.com/)
[![CMake](https://img.shields.io/badge/CMake-3.20.5%2B-44CC11?style=flat&logo=cmake&logoColor=white)](https://cmake.org/)
[![Python](https://img.shields.io/badge/Python-3.10%2B-FFD43B?style=flat&logo=python&logoColor=white)](https://www.python.org/)

本仓库展示了使用 GenOS 框架自动生成的物联网操作系统示例。本仓库展示了使用 GenOS 框架自动生成的物联网操作系统示例。它将 **Zephyr RTOS** 的实时处理能力与 **Trusted Firmware-M (TF‑M)** 的硬件级安全隔离结合，为工业物联网应用提供了一套参考实现与演示。  

---

## 📚 目录 (Table of Contents) 
- 🧩 [系统组成 (System Components)](#system-components) 
- 🛠️ [安装依赖 (Install Host Dependencies)](#install-dependencies-and-get-GenOS) 
- 📡 [样例应用演示：安全 I2C 访问 (Sample: Secure I2C Access)](#sample-secure-i2c-access) 
- ✍️ [开发者自定义规范 (Extension Rules)](#extension-rules) 
- 🎯 [结语 (Conclusion)](#conclusion) 

---

<h2 id="system-components">🧩 系统组成 (System Components) </h2>

下面是本样例系统的主要组件与职责说明：

| 组件名称 (Component) | 角色 (Role) | 仓库 / 描述 (Repo / Description) |
|---|---:|---|
| Manifest| 指挥部 🗂️ | 维护整个系统清单文件 `west.yml`，定义各组件版本与依赖关系。 |
| Zephyr Kernel | 实时内核 🧠 | 负责多线程调度、设备驱动与网络协议栈（非安全侧）。 |
| TF‑M | 安全底座 🔒 | 运行在安全侧，处理敏感外设访问、密钥与加密运算（Trusted Firmware‑M）。 |
| GenOS_Apps | 业务方案 📦 | 包含演示用的非安全侧应用与对应的安全侧 Partition 示例。 |

---

<h2 id="install-dependencies-and-get-GenOS">🛠️ 安装依赖 (Install Host Dependencies)</h2>

### 0. 系统依赖 (System dependencies)

💡 系统环境提示 (System Note): 本系统在 **Ubuntu 22.04.5 LTS (Kernel 6.8.0)** 上进行编译。当前主要依赖的最低版本需求如下：

| 工具 (Tool) | 最低版本 (Min. Version) |
|---|---:|
| CMake | 3.20.5 |
| Python | 3.10 |
| Devicetree compiler (dtc) | 1.4.6 |

在 Ubuntu 上可以使用 apt 安装以下所需软件包：

```bash
sudo apt install --no-install-recommends git cmake ninja-build gperf \
  ccache dfu-util device-tree-compiler wget python3-dev python3-venv python3-tk \
  xz-utils file make gcc gcc-multilib g++-multilib libsdl2-dev libmagic1
```

验证已安装的主要依赖版本：

```bash
cmake --version
python3 --version
dtc --version
```

将输出版本与上表中的最低版本进行比对。
初始化 GenOS 生成的 OS 工作区 (Initialize GenOS Workspace)
### 1. 获取 GenOS 生成的 OS 并安装 Python 依赖 (Get the Generative OS and install Python dependencies)

接下来，将 Zephyr 及其模块克隆到一个新的 west workspace 中。下面示例使用 `GenOS` 作为 workspace 名称，实际可以按需更换路径与名称。你还需要在 Python 虚拟环境中安装 Zephyr 的额外 Python 依赖。

创建一个新的虚拟环境：

```bash
python3 -m venv ~/GenOS/.venv
```

激活虚拟环境：

```bash
source ~/GenOS/.venv/bin/activate
```

激活后，命令提示符会以 `(.venv)` 为前缀。若要退出虚拟环境，可运行 `deactivate`。注意：每次开始工作时请记得激活该虚拟环境。

安装 west 工具：

```bash
pip install west
```

获取 GenOS 生成的 OS 源代码：

```bash
# 1. 在指定目录 (~/GenOS) 初始化 GenOS 空间
west init -m https://github.com/SZXtiantian/GenOS.git --mr zephyr ~/GenOS

# 2. 进入该目录
cd ~/GenOS

# 3. 同步依赖组件
west update
```

导出 Zephyr 的 CMake 包，这样 CMake 能自动加载构建 Zephyr 应用所需的样板代码：

```bash
west zephyr-export
```

使用 west 管理的 Python 包安装 Zephyr 的 Python 依赖：

```bash
west packages pip --install
```

注意：上述命令可能会升级或降级已安装的 `west` 版本，这是正常行为。

### 2. 安装对应的 SDK (Install the SDK)

Zephyr SDK 包含针对 Zephyr 支持的各个架构的交叉编译工具链（编译器、汇编器、链接器等），以及在 Linux 下用于仿真、烧写和调试所需的主机工具（如定制的 QEMU、OpenOCD 等）。

在已初始化的 workspace 中使用 west 安装 Zephyr SDK：

```bash
cd ~/GenOS/zephyr
west sdk install
```

提示：
- 使用命令选项可以指定 SDK 的安装目录以及需要安装的体系结构的工具链，参见 `west sdk install --help` 获取更多细节。
- 如果不想通过 `west sdk` 安装 SDK，也可以参照 Zephyr SDK 官方页面手动下载安装。

---

<h2 id="sample-secure-i2c-access">📡 样例应用演示：安全 I2C 访问 (Sample: Secure I2C Access)</h2>

此样例展示了 GenOS 框架下“硬件安全隔离”的典型场景。我们使用 **BH1750FVI（光强传感器）** 作为目标外设，演示如何跨越安全边界获取数据。

### 1. 硬件连接 (Hardware Connection)

在 nRF5340 DK 上，我们将 I2C 控制器分配给安全侧（TF-M），并连接 BH1750FVI 传感器：

| BH1750 引脚 | nRF5340 DK 引脚 | 说明 |
| :--- | :--- | :--- |
| **VCC** | **VDD (3.3V)** | 电源输入 |
| **GND** | **GND** | 接地 |
| **SDA** | **P0.02** | I2C 数据线 |
| **SCL** | **P0.03** | I2C 时钟线 |
| **ADDR** | **GND** | I2C 地址选择 (接地为 0x23) |


### 2. 系统交互逻辑 (System Logic)

- **安全侧 (TF‑M)** 🔑🔐：  
  在安全域中，使用 `tfm_partition` 驱动 I2C 硬件，安全域负责直接访问传感器并对敏感数据进行保护，不向非安全域开放直接硬件访问权限。

- **应用侧 (Zephyr / 非安全域)** 🔁📋：  
  通过 PSA API 向安全侧发起请求，由安全侧返回传感器数据或处理结果，实现受限硬件访问与权限控制。

构建示例（以 **nRF5340 DK** 开发板为例，双核架构）：
🛠 编译前准备

在使用本项目前，请修改 `~/GenOS/module/tee/tf-m/trusted-firmware-m/secure_fw/partitions/CMakeLists.txt` 中的路径配置以适配您的开发环境：

1. 打开 `~/GenOS/module/tee/tf-m/trusted-firmware-m/secure_fw/partitions/CMakeLists.txt`。
2. 找到以下部分并更新路径：

```cmake
# 将下方的路径修改为您本地的实际路径
set(TFM_EXTRA_PARTITION_PATHS "/<您的绝对路径>/GenOS_Apps/my_tfm_app1/tfm_partition" CACHE STRING "" FORCE)
set(TFM_EXTRA_MANIFEST_LIST_FILES "/<您的绝对路径>/GenOS_Apps/my_tfm_app1/tfm_partition/extra_manifest_list.yaml" CACHE STRING "" FORCE)
```
保存退出后返回GenOS目录下，构建非安全域应用镜像（系统会自动联动构建安全域固件）
```bash
# 返回GenOS目录
cd ~/GenOS
# 构建非安全域应用镜像（系统会自动联动构建安全域固件）
west build -b nrf5340dk/nrf5340/cpuapp/ns GenOS_Apps/my_tfm_app1/zephyr_app
```

构建完成后可将生成的固件刷入相应 CPU（非安全/安全）或使用开发板工具进行调试。🔧🖥️

---

<h2 id="extension-rules">✍️ 开发者自定义规范 (Extension Rules) </h2>

如果您希望基于此样例扩展自定义物联网方案，请遵循以下约定以保证与 GenOS 构建系统兼容：

- TODO 📝

（建议：在扩展时保持 manifest 清晰，遵循组件命名与分层约定，确保安全/非安全边界明确。）💡

---

<h2 id="conclusion">🎯 结语 (Conclusion) </h2>

GenOS 致力于简化复杂嵌入式系统的构建与集成流程。本示例展示了基础功能与最佳实践，更多高级特性（如 OTA 升级、多核通信、自动化 CI 流水线）请参考仓库文档或联系维护者了解详情。📌🔍

感谢使用 GenOS，欢迎反馈与贡献！🤝✨
