# MilliCar Weather ns-3 module 

This is an extension of the Millicar module with weather(dust/sand) impacts taken into consideration.

MilliCar is an [ns-3](https://www.nsnam.org "ns-3 Website") module that enables full stack end-to-end simulations in the Vehicle-to-Vehicle (V2V) networks operating in the millimeter wave (mmWave) frequency band.  

This project is still in progress and will change without notice.


## Getting Started
To use the module, you can clone this repository by:
```bash
git clone https://github.com/ericliujian/ns3-mmwave-weather.git
```

After clone, you can run several scripts directly:
```bash
./Name-of-file.sh
```

If there is an error " bad interpreter: No such file or directory". Please do:
```bash
find . -type f -print0 | xargs -0 dos2unix
```
It will recursively find all files inside current ns3 directory and call for these files dos2unix command to change to UNIX format.


##System specification:

The MilliCar extension and the examples were tested on the following system:

    ns-3: ns-3.33
    Ubuntu 20.04.2 LTS

If you see "collect2: error: ld returned 1 exit status", that might because you are using a older version of NS-3 on newer C++ compiler:

```bash
CXXFLAGS="-Wall" ./waf configure
./waf -vv
```

## Related modules


## References 
M. Drago, T. Zugno, M. Polese, M. Giordani, M. Zorzi, "Millicar - An ns-3 Module for MmWave NR V2X Networks," Proc. of the Workshop on ns-3 (WNS3), 2020.


## License ##

This software is licensed under the terms of the GNU GPLv2, as like as ns-3. See the LICENSE file for more details.
