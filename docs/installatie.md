# 1tracker_pi — Installatie-instructies (bèta-testers)

Bedankt voor het testen van **1tracker_pi**, een OpenCPN-plugin die je bootpositie
(en optioneel winddata) naar HTTP-endpoints verstuurt — zoals NoForeignLand of een
eigen server.

---

## Vereisten

- OpenCPN **5.6 of nieuwer**
- Besturingssysteem: Linux, macOS of Windows
- Internetverbinding tijdens installatie

---

## Stap 1 — Aangepaste catalogus inschakelen

OpenCPN blokkeert standaard het toevoegen van externe catalogi. Dit moet je eenmalig
handmatig inschakelen door een regel toe te voegen aan het configuratiebestand.

### Configuratiebestand openen

De makkelijkste manier om het configuratiebestand te vinden:

1. Klik in de OpenCPN-werkbalk op het **?-icoon**
2. Kies **About OpenCPN**
3. Je ziet daar de exacte naam en locatie van het configuratiebestand — klik erop om het direct te openen

> **Standaardlocaties ter referentie:**
> - macOS: `~/Library/Preferences/opencpn/opencpn.conf`
> - Linux: `~/.opencpn/opencpn.conf`
> - Windows: `%APPDATA%\opencpn\opencpn.ini`

1. **Sluit OpenCPN volledig af**
2. Open het configuratiebestand in een teksteditor
3. Zoek de sectie `[PlugIns]` (als die er niet is, voeg hem toe onderaan)
4. Voeg deze regel toe onder `[PlugIns]`:

```
CatalogExpert=1
```

5. Sla het bestand op en **open OpenCPN opnieuw**

---

## Stap 2 — Alpha-catalogus toevoegen

Nu de expertmodus is ingeschakeld:

1. Ga naar **Opties** → tabblad **Plugins**
2. Klik op **Catalogus bijwerken** (*Update catalog*)
3. Er verschijnt nu een veld voor een **aangepaste URL** — voer in:

```
https://dl.cloudsmith.io/public/pa2wlt/1tracker-alpha/raw/files/ocpn-plugins.xml
```

4. Klik op **OK** en daarna opnieuw op **Catalogus bijwerken**

---

## Stap 3 — Plugin installeren

1. Zoek in de pluginlijst naar **1tracker**
2. Klik op **Installeren**
3. Herstart OpenCPN wanneer hierom gevraagd wordt

---

## Stap 4 — Plugin activeren

1. Ga opnieuw naar **Opties** → **Plugins**
2. Zoek **1tracker** in de lijst en klik op **Inschakelen** (*Enable*)
3. Klik **OK** en herstart OpenCPN

---

## Stap 5 — Configureren

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
