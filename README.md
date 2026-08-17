# 🚨 Railway Bridge Water Level Management System

## Overview
A command-line application to monitor and track water levels at railway bridges with danger alerts.

## Features
- 📊 Real-time water level monitoring
- ⚠️ Automatic danger alerts (>30cm threshold)
- 📈 Statistics & historical tracking
- 📉 ASCII graph visualization
- 🔴 Danger log maintenance

## 📋 Requirements
- **OS**: Windows (current version) / Linux (with modifications)
- **Compiler**: GCC or MSVC
- **C Standard**: C99 or later

## 🛠️ Compilation & Running

**Windows:**
```bash
gcc -o RBWMS.exe RBWMS.c
./RBWMS.exe
```

**Linux/Mac** (after modifications):
```bash
gcc -o RBWMS RBWMS.c
./RBWMS
```

## 📸 Screenshots
[Add 3-4 images here showing: Main menu, entering data, viewing stats, danger alert]

## 🎮 Usage
1. Select a bridge (A, B, or C)
2. Enter water level reading
3. System automatically alerts if above 30cm
4. View statistics and danger logs

## 📁 File Structure
- `bridgeA.txt` - Water level readings for Bridge A
- `bridgeA_danger.txt` - Danger events log

## 🚀 Future Improvements
- [ ] Cross-platform compatibility
- [ ] Database instead of text files
- [ ] Real-time alerts/notifications
- [ ] Web dashboard
- [ ] Data export (CSV/JSON)

## 👨‍💻 Author
Yash Kharabe

## 📝 License
MIT
