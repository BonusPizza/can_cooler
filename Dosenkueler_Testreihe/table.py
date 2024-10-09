import json
import matplotlib.pyplot as plt


# Datei laden
file_path = 'Dosenkueler_Testreihe/nur_dose.json'
file_path_cooler = 'Dosenkueler_Testreihe/dosenkueler_v1.json'

# JSON-Dateien laden
with open(file_path, 'r') as f:
    data = json.load(f)

with open(file_path_cooler, 'r') as f:
    data_cooler = json.load(f)

# Zeitstempel und Temperaturen extrahieren
timestamps = list(data.keys())
air_temp = [float(data[time]['airTemp']) for time in timestamps]
liquid_temp = [float(data[time]['liquidTemp']) for time in timestamps]

air_temp_cooler = [float(data_cooler[time]['airTemp']) for time in timestamps]
liquid_temp_cooler = [float(data_cooler[time]['liquidTemp']) for time in timestamps]

# Zeit in Minuten umwandeln
time_in_minutes = [int(time.split(':')[1]) + int(time.split(':')[0]) * 60 for time in timestamps]

# Diagramme erstellen
plt.figure(figsize=(10, 5))

# Diagramm ohne Kühler
plt.plot(time_in_minutes, air_temp, label='Air Temperature (°C) without Cooler', color='blue')
plt.plot(time_in_minutes, liquid_temp, label='Liquid Temperature (°C) without Cooler', color='red')


# Diagramm mit Kühler
plt.plot(time_in_minutes, air_temp_cooler, label='Air Temperature (°C) with Cooler', color='orange')
plt.plot(time_in_minutes, liquid_temp_cooler, label='Liquid Temperature (°C) with Cooler', color='green')

# Diagrammgestaltung
plt.xlabel('Time (Minutes)')
plt.ylabel('Temperature (°C)')
plt.title('Air and Liquid Temperature over Time')
plt.legend()
plt.grid(True)
plt.tight_layout()



# Diagramm anzeigen
plt.savefig('Dosenkueler_Testreihe/Can_Cooler_v1.png')
plt.show()
