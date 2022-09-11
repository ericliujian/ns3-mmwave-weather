# MilliCar Weather ns-3 module 

This is an extension of the Millicar module with weather(dust/sand) impacts taken into consideration.

MilliCar is an [ns-3](https://www.nsnam.org "ns-3 Website") module that enables full stack end-to-end simulations in the Vehicle-to-Vehicle (V2V) networks operating in the millimeter wave (mmWave) frequency band.
We extend it by adding weather impacts into path loss functions.  

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


## System specification:

The MilliCar extension and the examples were tested on the following system:

    ns-3: ns-3.33
    Ubuntu 18.04.6 LTS and 20.04.4 LTS

If you see "collect2: error: ld returned 1 exit status", that might because you are using a older version of NS-3 on newer C++ compiler:

```bash
CXXFLAGS="-Wall" ./waf configure
./waf -vv
```

## References 
M. Drago, T. Zugno, M. Polese, M. Giordani, M. Zorzi, "Millicar - An ns-3 Module for MmWave NR V2X Networks," Proc. of the Workshop on ns-3 (WNS3), 2020.

## Cite our work
If you find our work is useful, please use the following bibtex:
```
@ARTICLE{9739727,  
         author={Abuhdima, Esmail and Liu, Jian and Zhao, Chunheng and Elqaouaq, Ahmed and Comert, Gurcan and Huang, Chin-Tser and Pisu, Pierluigi and Nazeri, Amir Hossein},  
         journal={IEEE Journal of Radio Frequency Identification},   
         title={Impact of Dust and Sand on 5G Communications for Connected Vehicles Applications},   
         year={2022},  
         volume={6},  
         number={},  
         pages={229-239},  
         doi={10.1109/JRFID.2022.3161391}}
```
## License ##

This software is licensed under the terms of the GNU GPLv2, as like as ns-3. See the LICENSE file for more details.
