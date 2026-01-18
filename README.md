# GenOS 物联网操作系统：功能演示样例 (Sample Project) 🚀

本仓库展示了使用 GenOS 框架自动生成的物联网操作系统示例。它将 Zephyr RTOS 的实时处理能力与 Trusted Firmware-M (TF‑M) 的硬件级安全隔离结合，为工业物联网应用提供了一套参考实现与演示。🧩🔒

---

## 目录 (Table of Contents) 📚
- [系统组成 (System Components)](#系统组成-system-components) 🧭
- [快速部署 (Quick Deployment)](#快速部署-quick-deployment) ⚡
- [样例应用演示：安全 I2C 访问 (Sample: Secure I2C Access)](#样例应用演示安全-i2c-访问-sample-secure-i2c-access) 🔐📡
- [开发者自定义规范 (Extension Rules)](#开发者自定义规范-extension-rules) ✍️
- [结语 (Conclusion)](#结语-conclusion) 🎯

---

## 系统组成 (System Components) 🧩

下面是本样例系统的主要组件与职责说明：

| 组件名称 (Component) | 角色 (Role) | 仓库 / 描述 (Repo / Description) |
|---|---:|---|
| Manifest (GenOS) | 指挥部 🗂️ | 维护整个系统清单文件 `west.yml`，定义各组件版本与依赖关系。 |
| Zephyr Kernel | 实时内核 🧠 | 负责多线程调度、设备驱动与网络协议栈（非安全侧）。 |
| TF‑M (SPE) | 安全底座 🔒 | 运行在安全侧，处理敏感外设访问、密钥与加密运算（Trusted Firmware‑M）。 |
| GenOS_Apps | 业务方案 📦 | 包含演示用的非安全侧应用与对应的安全侧 Partition 示例。 |

---

## 快速部署 (Quick Deployment) ⚡

在本地复现该示例系统只需两步：

```bash
# 1. 初始化 GenOS 演示空间（使用 manifest）
west init -m https://github.com/SZXtiantian/GenOS.git --mr zephyr

# 2. 同步依赖组件
west update
```

执行完上述命令后，仓库会按 manifest 中定义拉取 Zephyr、TF‑M、各模块与示例应用代码。🛠️✅

---

## 样例应用演示：安全 I2C 访问 (Sample: Secure I2C Access) 🔐📡

此样例展示“安全域驱动外设”的典型场景：

- 安全侧 (TF‑M)：  
  在安全域中，使用 `tfm_partition` 驱动 I2C 硬件，安全域负责直接访问传感器并对敏感数据进行保护，不向非安全域开放直接硬件访问权限。🔑🧪

- 应用侧 (Zephyr / 非安全域)：  
  通过 PSA API 向安全侧发起请求，由安全侧返回传感器数据或处理结果，实现受限硬件访问与权限控制。🔁📋

构建示例（以 nRF5340 开发板为例，双核架构）：

```bash
# 构建非安全域应用镜像（系统会自动联动构建安全域固件）
west build -b nrf5340dk/nrf5340/cpuapp/ns GenOS_Apps/my_tfm_app1/zephyr_app
```

构建完成后可将生成的固件刷入相应 CPU（非安全/安全）或使用开发板工具进行调试。🔧🖥️

---

## 开发者自定义规范 (Extension Rules) ✍️

如果您希望基于此样例扩展自定义物联网方案，请遵循以下约定以保证与 GenOS 构建系统兼容：

- TODO 📝

（建议：在扩展时保持 manifest 清晰，遵循组件命名与分层约定，确保安全/非安全边界明确。）💡

---

## 结语 (Conclusion) 🎯

GenOS 致力于简化复杂嵌入式系统的构建与集成流程。本示例展示了基础功能与最佳实践，更多高级特性（如 OTA 升级、多核通信、自动化 CI 流水线）请参考仓库文档或联系维护者了解详情。📌🔍

感谢使用 GenOS，欢迎反馈与贡献！🤝✨
