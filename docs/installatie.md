# 1tracker_pi — Installatie-instructies (bèta-testers)

Bedankt voor het testen van **1tracker_pi**, een OpenCPN-plugin die je bootpositie
(en optioneel winddata) naar HTTP-endpoints verstuurt — zoals NoForeignLand of een
eigen server.

---

## Vereisten

- OpenCPN **5.6 of nieuwer**
- Internetverbinding tijdens installatie

---

## Stap 1 — Download de plugin

Kies de versie die bij jouw besturingssysteem past:

| Besturingssysteem | Download |
|---|---|
| **macOS** (Apple Silicon én Intel) | [1tracker-0.1.0_darwin-universal.tar.gz](https://dl.cloudsmith.io/public/pa2wlt/1tracker-alpha/raw/names/1tracker-0.1-darwin-wx32-universal-10.15-tarball/versions/0.1.0+452.5923a13/1tracker-0.1.0+452.5923a13_darwin-wx32-10.15-arm64-x86_64.tar.gz) |
| **Windows** | [1tracker-0.1.0_windows.tar.gz](https://dl.cloudsmith.io/public/pa2wlt/1tracker-alpha/raw/names/1tracker-0.1-msvc-wx32-10-tarball/versions/0.1.0+21.93891e0/1tracker-0.1.0+21.93891e0_msvc-wx32-10-x86.tar.gz) |
| **Linux** (Debian/Ubuntu 64-bit) | [1tracker-0.1.0_debian12-x86_64.tar.gz](https://dl.cloudsmith.io/public/pa2wlt/1tracker-alpha/raw/names/1tracker-0.1-debian-x86_64-12-tarball/versions/0.1.0+599.db96f13/1tracker-0.1.0+599.db96f13_debian-x86_64-12-x86_64.tar.gz) |
| **Android** (64-bit, meeste telefoons) | [1tracker-0.1.0_android-arm64.tar.gz](https://dl.cloudsmith.io/public/pa2wlt/1tracker-alpha/raw/names/1tracker-0.1-android-arm64-A64-16-tarball/versions/0.1.0+600.db96f13/1tracker-0.1.0+600.db96f13_android-arm64-16-arm64.tar.gz) |
| **Android** (32-bit, oudere telefoons) | [1tracker-0.1.0_android-armhf.tar.gz](https://dl.cloudsmith.io/public/pa2wlt/1tracker-alpha/raw/names/1tracker-0.1-android-armhf-A32-16-tarball/versions/0.1.0+594.db96f13/1tracker-0.1.0+594.db96f13_android-armhf-16-armhf.tar.gz) |

---

## Stap 2 — Installeer de plugin

1. Open OpenCPN
2. Ga naar **Opties** → tabblad **Plugins**
3. Klik op **Installeer plugin uit bestand** (of *Install plugin from file*)
4. Selecteer het gedownloade `.tar.gz` bestand
5. Herstart OpenCPN wanneer hierom gevraagd wordt

---

## Stap 3 — Plugin activeren

1. Ga opnieuw naar **Opties** → **Plugins**
2. Zoek **1tracker** in de lijst en klik op **Inschakelen** (*Enable*)
3. Klik **OK** en herstart OpenCPN

---

## Stap 4 — Configureren

Na het herstarten verschijnt een nieuw icoon in de werkbalk.

1. Klik op het **1tracker-icoon** in de werkbalk om het configuratievenster te openen
2. Klik op **Endpoint toevoegen** en vul in:

| Veld | Omschrijving |
|---|---|
| **Naam** | Vrij te kiezen, bv. `mijn-nfl-tracker` |
| **Type** | `JSON` (generiek HTTP) of `NoForeignLand` |
| **URL** | De endpoint-URL van jouw tracker-dienst |
| **API-sleutel / Header** | Indien vereist door de dienst |
| **Interval** | Hoe vaak de positie wordt verstuurd (in minuten) |

3. Zet de schakelaar op **Aan**
4. Klik **Opslaan**

### Werkbalk-icoon

| Kleur | Betekenis |
|---|---|
| 🟢 Groen | Laatste verzending geslaagd |
| 🔴 Rood | Fout bij laatste verzending |
| ⚪ Grijs | Inactief of nog geen verzending |

---

## Problemen of feedback?

Meld bugs en opmerkingen via GitHub:
[github.com/pa2wlt/1tracker_pi/issues](https://github.com/pa2wlt/1tracker_pi/issues)

Of neem direct contact op met de ontwikkelaar.

---

*Versie 0.1.0 — april 2026*
