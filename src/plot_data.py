import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from datetime import datetime, timedelta


#load data
#add more text files if necessary
df1 = np.loadtxt(r"[File Path for Gadget 1 text file goes here, remove brackets]", delimiter = ', time: ')
df2 = np.loadtxt(r"[File Path for Gadget 2 text file goes here, remove brackets]", delimiter = ', time: ')
df3 = np.loadtxt(r"[File Path for Gadget 3 text file goes here, remove brackets]", delimiter = ', time: ')

#since there are delays, include delay times to line up the data. Trial and error process.
starttime_delay_1 = 0
starttime_delay_2 = (7*60 -55-1.6)*1000   #millisecon
starttime_delay_4 = (14*60 + 343 - 1.6) * 1000   #millisecond


#convert from milisecond to second and line up
df1[:, 1] = df1[:, 1] + starttime_delay_1 
df2[:, 1] = df2[:, 1] + starttime_delay_2
df3[:, 1] = df3[:, 1] + starttime_delay_4 

starttime = datetime(2022, 5, 6, 16, 46, 0)

dt1 = starttime + df1[:, 1] * timedelta(milliseconds = 1)
dt2 = starttime + df2[:, 1] * timedelta(milliseconds = 1)
dt3 = starttime + df3[:, 1] * timedelta(milliseconds = 1)

#plot
fig, ax = plt.subplots(1, 1, figsize=(10, 8))
ax.plot(dt1, df1[:, 0], 'r', dt2, df2[:, 0], 'g', \
        dt3, df3[:, 0], 'b')

ax.grid(True)
ax.set_xlabel('Date time')
ax.set_ylabel('Device digital output (0-1024)')
ax.legend(['Gadget-1', 'Gadget-2', 'Gadget-3'], loc='upper left')

#Convert to CSV: Optional

#dev1 = pd.concat([pd.DataFrame(data=dt1), pd.DataFrame(data=df1[:, 0])], axis=1, join="inner")
#dev1.to_csv("dev1.csv", date_format='%s', header=False, index=False)
#
#dev2 = pd.concat([pd.DataFrame(data=dt2), pd.DataFrame(data=df2[:, 0])], axis=1, join="inner")
#dev2.to_csv("dev2.csv", date_format='%s', header=False, index=False)
#
#dev3 = pd.concat([pd.DataFrame(data=dt4), pd.DataFrame(data=df4[:, 0])], axis=1, join="inner")
#dev3.to_csv("dev4.csv", date_format='%s', header=False, index=False)

plt.show()

