import json
from pykalman import KalmanFilter
import matplotlib.pyplot as plt
import numpy as np
with open("1.txt", "r") as f:
    data = [json.loads(line) for line in f]
rssi = [d["rssi"] for d in data]
kf = KalmanFilter(transition_matrices=[1],
                  observation_matrices=[1],
                  initial_state_mean=rssi[0],
                  initial_state_covariance=1,
                  observation_covariance=1,
                  transition_covariance=0.02)


rssi_filtered = kf.filter(rssi)
rssi_filtered_list = []
for value in rssi_filtered[0].tolist():
    rssi_filtered_list.append(value[0])
print(rssi_filtered_list)
plt.plot(rssi, 'b-', label='Dados originais')
plt.plot(rssi_filtered_list, 'r-', label='Dados suavizados')
plt.legend()
plt.show()
