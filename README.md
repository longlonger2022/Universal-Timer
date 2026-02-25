# Universal-Timer / 万能倒计时

![GPLv3 License](https://img.shields.io/badge/license-GPLv3-blue.svg)
![Qt](https://img.shields.io/badge/Qt-6.10.1-green.svg)
![Version](https://img.shields.io/badge/version-2.0.0--beta.4-orange)

**Universal-Timer** (formerly known as UniversalTimer2) is a powerful and customizable countdown application built with Qt. It helps you track important events by displaying the remaining time in a floating bar and providing full-screen reminders with animations and sounds. This is the **2.0.0-beta.4** release, completely rewritten from the 1.x series, offering more features and better performance than its predecessor.


**万能倒计时**（原项目名 UniversalTimer2）是一个基于 Qt 的强大且可定制的倒计时应用程序。它通过在悬浮条中显示剩余时间，并提供带有动画和音效的全屏提醒，帮助您追踪重要事件。这是 **2.0.0-beta.4** 版本，完全由 1.x 系列重写而来，将具有比 1.x 更多的功能和更好的性能。

---

## Table of Contents / 目录

- [Features / 功能特性](#features--功能特性)
- [Screenshots / 截图](#screenshots--截图)
- [Usage / 使用方法](#usage--使用方法)
- [Configuration / 配置](#configuration--配置)
- [License / 许可证](#license--许可证)
- [Contributing / 贡献](#contributing--贡献)
- [Contact / 联系方式](#contact--联系方式)

---

## Features / 功能特性

- **Floating countdown bar** – Always on top (optional), with customizable text, size, and position, keeping you informed at a glance.
- **Full-screen reminder** – Appears at scheduled times or on demand, featuring red block animations and sound alerts to draw your attention.
- **Customizable texts** – Personalize the main text, small text, and countdown format to suit your needs.
- **Sound reminders** – Plays countdown or heartbeat sounds when the remaining days fall below your set thresholds, for effective alerts.
- **Tray icon** – Provides quick access to settings, refresh, and exit, for convenience.
- **Persistent configuration** – All settings are automatically saved in `config.ini` and reloaded on next startup. You can also edit the file manually for advanced customization.
- **Scheduled full-screen reminders** – Automatically triggers reminders at predefined times of the day (configurable in `timelist` file), helping you stay on track.
- **Welcome screen** – Guides first-time users through initial setup, making it easy to get started.

- **悬浮倒计时条** – 可选的置顶显示，支持自定义文本、大小和位置，剩余时间一目了然。
- **全屏提醒** – 在预定时间或手动触发时显示，伴有红色方块动画和声音提示，时刻提醒您的重要事件。
- **自定义文本** – 修改主文本、小文本以及倒计时格式，满足您的个性化需求。
- **声音提醒** – 当剩余天数低于阈值时播放倒计时或心跳音，更加有效地提醒您。
- **托盘图标** – 快速访问设置、刷新和退出，方便快捷。
- **配置持久化** – 所有设置保存在 `config.ini` 中，确保下次启动时自动加载，无需重新配置。您也可随时手动修改，自由定制。
- **定时显示全屏提醒** – 在一天中的预设时间自动显示全屏提醒，让您在过度放松时也能及时提醒自己。您可以随时在`timelist`文件中添加或删除时间，实现灵活的提醒管理。
- **欢迎界面** – 首次使用时提供引导，帮助您快速上手，轻松开始。

---

## Screenshots / 截图

*Screenshots will be added soon.*  
*截图即将添加。*

---

## Usage / 使用方法

### First Run / 首次运行
- The welcome screen appears. Click **Start** to proceed.
- Default target is set to **June 30, 2025**; you can change it later in settings.

- 出现欢迎界面，点击 **开始** 继续。
- 默认日期为2025年6月30日，您可以在设置中随时更改。

### Tray Icon / 托盘图标
- Right-click the tray icon to open the menu:
  - **Settings** – Open full-screen settings panel.
  - **Refresh** – Reload configuration from disk.
  - **Exit** – Quit the application.

- 右键点击托盘图标，打开菜单：
  - **设置** – 打开全屏设置面板。
  - **刷新** – 从磁盘重新加载配置。
  - **退出** – 退出应用程序。

### Full-Screen Reminder / 全屏提醒
- Automatically triggered when the current time matches any entry in the `timelist` file.
- You can also preview it from the settings panel.
- When the remaining days are low, the reminder will flash red blocks and play sounds.

- `timelist` 文件中的时间会自动触发全屏提醒。
- 您也可以在设置面板中预览提醒。
- 当剩余天数低于阈值时，提醒会闪烁红色方块并播放声音。

### Customization / 自定义
- Open **Settings** from the tray menu to adjust:
  - Target date/time
  - Floating bar text, height, position, and always-on-top behavior
  - Full-screen reminder texts and trigger thresholds
  - Number of flash cycles and sound options
- All changes are saved immediately.

- 从托盘菜单中打开 **设置**，调整以下内容：
  - 目标日期 / 时间
  - 悬浮条文本、高度、位置和置顶行为
  - 全屏提醒文本和触发阈值
  - 闪烁周期和声音选项
- 所有更改会立即保存。

---

## Configuration / 配置
- `config.ini` – Stores all user settings (INI format).
- `timelist` – Text file with one time per line (HH:mm:ss) to trigger full-screen reminders.
- `Universal-Timer.log` – Log file for debugging.
- `sound/` – Folder containing `countdown.wav` and `heartbeat.wav`.

You can edit these files manually, but using the settings panel is recommended.

- `config.ini` – 存储所有用户设置（INI 格式）。
- `timelist` – 每行一个时间（HH:mm:ss）的文本文件，用于触发全屏提醒。
- `Universal-Timer.log` – 日志文件。
- `sound/` – 包含 `countdown.wav` 和 `heartbeat.wav` 的文件夹。

您可以手动编辑这些文件，但建议使用设置面板进行操作。

---

## License / 许可证

This project is licensed under the **GNU General Public License v3.0**. See the [LICENSE](LICENSE) file for details.  
本项目采用 **GNU 通用公共许可证 v3.0** 授权。详情请参阅 [LICENSE](LICENSE) 文件。

---

## Contributing / 贡献

Contributions are welcome! Please open an issue or submit a pull request.  
欢迎贡献！请提出 issue 或提交 pull request。

---

## Contact / 联系方式

- Author / 作者: 龙ger_longer
- GitHub: [@longlonger2022](https://github.com/longlonger2022)

---

*Coming soon...*

*即将推出……*