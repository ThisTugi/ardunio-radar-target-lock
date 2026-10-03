# 🎯 Autonomous Ultrasonic Radar & Target-Lock System

An embedded radar prototype built with Arduino Uno that continuously scans a 150-degree horizontal field and automatically locks onto targets entering a predefined proximity threshold, featuring signal noise debouncing and state management.

---

## 📌 Project Overview

This project implements an autonomous target acquisition system using a pan-axis servo mechanism and an ultrasonic distance sensor. 

The system operates across two discrete functional states:
1. **Searching Mode:** The servo continuously sweeps between 15° and 165° in 3° steps to map proximity data. The system indicates this state via a green status LED.
2. **Hunting (Locked) Mode:** Once an object is verified within the critical engagement zone (5 cm to 25 cm), the sweep stops immediately. The servo holds its angular position facing the target, and the status switches to a red warning LED.

---

## ⚙️ Key Technical Features

* **Bi-directional Servo Sweep:** Smooth 15°–165° and 165°–15° pan-axis tracking implemented with structured sub-routine calls (`scan()`).
* **Dual-Layer Echo Debounce:** Prevents false triggers and servo jitter caused by ultrasonic signal scatter:
  * **Acquisition Confirmation:** Requires consecutive matching samples before engaging target lock.
  * **Lost-Target Buffer:** Utilizes a consecutive failure counter before releasing the lock and resuming sweep mode.
* **Modular & DRY Architecture:** All position writing, signal acquisition, and state management logic are centralized in a single reusable function.

---

## 🛠️ Hardware Requirements & Pinout

| Component | Arduino Pin | Description |
| :--- | :--- | :--- |
| **HC-SR04 Trigger** | Digital Pin 6 | Ultrasonic pulse trigger |
| **HC-SR04 Echo** | Digital Pin 7 | Echo return pulse measurement |
| **SG90 Servo Motor** | Digital Pin 3 | PWM-controlled pan movement |
| **Green Status LED** | Digital Pin 9 | Indicates Searching Mode |
| **Red Status LED** | Digital Pin 10 | Indicates Target-Locked Mode |
| **Current Limiting Resistors** | — | 220Ω / 330Ω for status LEDs |
| **Breadboard & Jumper Wires** | — | Circuit assembly |

---

## 🧠 System Architecture & State Logic

```text
       +-----------------------+
       |   Searching Mode      | <-------+
       |  (Green LED / Sweep)  |         |
       +-----------+-----------+         |
                   |                     |
           Target Detected?              |
          (5 cm <= d <= 25 cm)           |
                   |                     |
                   v                     |
       +-----------------------+         |
       |     Hunting Mode      |         |
       |  (Red LED / Locked)   |         |
       +-----------+-----------+         |
                   |                     |
           Target Lost for               |
          >= 2 cycles (Buffer)           |
                   |                     |
                   +---------------------+
