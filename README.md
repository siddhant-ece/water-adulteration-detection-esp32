# 💧 Water Adulteration Detection System
### ESP32 | IoT | Machine Learning | Blynk | TDS & Turbidity Sensors

---

## 📌 Overview
A cost-effective, real-time Water Quality Monitoring System built using the ESP32 Microcontroller
and digital sensors (TDS & Turbidity). Machine Learning models classify water quality as
**Safe**, **Moderately Polluted**, or **Severely Contaminated**, and results are pushed
to a smartphone via the **Blynk IoT platform** over Wi-Fi.

---

## ⚙️ Technologies Used
| Category | Tools / Tech |
|---|---|
| Microcontroller | ESP32 |
| Sensors | TDS Sensor, Turbidity Sensor |
| IoT Platform | Blynk (Wi-Fi) |
| ML Framework | Python, Scikit-learn (Random Forest) |
| Language | C (Arduino / ESP-IDF), Python |
| IDE | Arduino IDE / VS Code |

---

## 🔌 Hardware Components
- ESP32 Development Board
- TDS (Total Dissolved Solids) Sensor Module
- Turbidity Sensor Module
- Alert LED (Red)
- Wi-Fi Network
- Smartphone with Blynk App

---

## 🗂️ Pin Configuration
| Component | ESP32 Pin |
|---|---|
| TDS Sensor | GPIO 34 (Analog) |
| Turbidity Sensor | GPIO 35 (Analog) |
| Alert LED | GPIO 2 |

---

## 🤖 How It Works
1. TDS and Turbidity sensors continuously measure water parameters
2. ESP32 reads analog values and converts them to ppm and NTU units
3. ML model (Random Forest) classifies water into 3 categories
4. Results are sent to the **Blynk App** in real-time over Wi-Fi
5. If water is polluted or contaminated, an **alert LED** turns ON and a **push notification** is sent

---

## 📊 Water Quality Classification
| Category | TDS (ppm) | Turbidity (NTU) |
|---|---|---|
| ✅ Safe | < 300 | < 10 |
| ⚠️ Moderately Polluted | 300 – 600 | 10 – 50 |
| ❌ Severely Contaminated | > 600 | > 50 |

---

## 📱 Blynk Virtual Pin Mapping
| Virtual Pin | Data |
|---|---|
| V0 | TDS Value (ppm) |
| V1 | Turbidity Value (NTU) |
| V2 | Quality Code (0/1/2) |
| V3 | Quality Label (Text) |

---

## 🗂️ Repository Structure
```
water-adulteration-detection-esp32/
├── water_quality_monitor.ino   # ESP32 Main Source Code (C)
├── ml_model.py                 # ML Model Training Script (Python)
├── water_quality_model.pkl     # Trained ML Model (generated after running ml_model.py)
└── README.md                   # Project Documentation
```

---

## 🚀 Getting Started

### 1. Clone the Repository
```bash
git clone https://github.com/YourUsername/water-adulteration-detection-esp32.git
```

### 2. Install Arduino Libraries
- `BlynkSimpleEsp32`
- `WiFi` (built-in ESP32)

### 3. Configure Credentials
Open `water_quality_monitor.ino` and update:
```c
#define BLYNK_TEMPLATE_ID  "YourTemplateID"
#define BLYNK_AUTH_TOKEN   "YourAuthToken"
const char* ssid     = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
```

### 4. Train the ML Model
```bash
pip install pandas scikit-learn joblib
python ml_model.py
```

### 5. Flash to ESP32
Upload `water_quality_monitor.ino` via Arduino IDE to your ESP32 board.

---

## 👤 Author
**Siddhant Singh**
B.Tech — Electronics & Communication Engineering
Lovely Professional University
