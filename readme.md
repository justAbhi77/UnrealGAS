# UnrealGAS – Multiplayer RPG with GAS - UE5.3+

A basic RPG sample built using Unreal Engine’s **Gameplay Ability System (GAS)**.  
Supports a full multiplayer architecture with a host-save system, diverse attribute sets, gameplay effects, cues, and more.

---

## 🎮 Features

- **Attribute System**
  - Vital Attributes: Health, Mana
  - Non-Vital Attributes: Mana Regen, Stamina
  - Tertiary Attributes: Damage, Experience, Defense, etc.
- **Gameplay Abilities & Effects**
  - Fully featured **Gameplay Effects** (duration, instant, infinite)
  - **Gameplay Cues**: Static and burst (for VFX/SFX triggering)
  - Dynamic scaling using `SetByCaller` values
- **Multiplayer-Ready**
  - Built and tested in a dedicated server + client setup
  - Supports replicated abilities and prediction windows
- **Host-Based Save System**
  - Save/load attribute state, experience, and other key gameplay data on the host machine

---

## 🧱 System Overview

### 🔧 Attribute Sets
Attributes are categorized and modularized into different sets:
- `VitalAttributes`: Health, Mana, etc.
- `SecondaryAttributes`: Stamina Regen, Speed
- `TertiaryAttributes`: Damage, Critical Chance, etc.

All sets derive from `UAttributeSet` and integrate with `UAbilitySystemComponent`.

### ⚡ Abilities & Effects
- Abilities are triggered via input and managed by the Ability System Component
- Effects use `GameplayEffectExecutionCalculation` for damage logic
- Cooldowns and costs implemented with standard GAS best practices

### 🎆 Gameplay Cues
- Cue Notifies for VFX/SFX (both static and burst)
- Triggered from GE application, ability activation, or tag changes

### 🌐 Multiplayer
- Server-authoritative gameplay
- Client prediction using `PredictionKey`
- Ability input via Enhanced Input framework
- Supports multiple players over LAN/internet with replication of attributes and state

### 💾 Save System
- Implemented via `SaveGame` and custom `USaveGame` subclasses
- Captures attribute values, XP, level, and other persistent data
- Saves only on the host for simplicity and security

---

## 🚀 Getting Started

### Option 1
- Clone the repo:
   ```bash
   git clone https://github.com/justAbhi77/UnrealGAS.git
   ```

- Open `Aura.uproject` in Unreal Engine 5.3+.
- Build using Visual Studio 2022 or Rider.
- Play the game.

### Option 2
- Download the Alpha Build
- Visit the Releases page
- Download the latest Alpha Release ZIP
- Extract and run the packaged game (Aura.exe)
- Multiplayer support currently only on Engine

---

## 🔍 Debugging Tips

- `showdebug abilitysystem` – Shows active abilities, effects, and tags
- Use the Gameplay Cue log (`LogGameplayCue`) for tracking VFX/SFX
- Add breakpoints to `ExecuteGameplayEffect` or `ApplyGameplayEffectToTarget` to trace logic

---

## 📖 Learn More

- [tranek/GASDocumentation](https://github.com/tranek/GASDocumentation)

- [Unreal Engine GAS Indie Tutorial](https://dev.epicgames.com/community/learning/tutorials/8Xn9/unreal-engine-epic-for-indies-your-first-60-minutes-with-gameplay-ability-system)

---

## 📝 License
This project is licensed under the MIT License.<br>
Feel free to use, modify, and redistribute for personal or commercial projects.

## 🙏 Acknowledgments
- [Stephen Ulibarri](https://www.udemy.com/user/stephen-ulibarri-3/)
