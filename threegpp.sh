#!/bin/bash


CXXFLAGS="-Wall" ./waf configure 


#change frequency to 0.7, 0.85,1.7,1.9,2.1 for 4G LTE, 28 39 47 73.5 for 5G 39e9 47e9 73.5e9, change to 5.9e9 for DSRC
frequency=28e9

txPow_dbm=15.0

noiseFigure=9.0

#three weather impacts, need to change in for loops
particleradius=0.0002
visibility=0.002
humidity=0

 
scenario1="V2V-Highway" # communication environment 
scenario2="V2V-Urban" # communication environment  


./waf --run "three-gpp-v2v-channel-example --frequency=$frequency --noiseFigure=$noiseFigure --scenario=$scenario1  --Pvalue=$particleradius --Vvalue=$visibility --Hvalue=$humidity" 


#three nested for loops works for setting up experiments for weather impacts, also add one for loop for frequency

<<comment
for frequency in 28e9 ; do
    for particleradius in $(seq 0.0000 0.00005 0.0009); do
        for visibility in $(seq 0.000 0.0005 0.005); do
            for humidity in $(seq 0 20 100); do
	        ./waf --run "5g_v2 --bandwidth=$bandwidth --frequency=$frequency --iip=$interPacketInterval --speed=$speed --intraGroupDistance=$intraGroupDistance --Pvalue=$particleradius --Vvalue=$visibility --Hvalue=$humidity --channel_condition=$environment1 --scenario=$commScenario1"            
            done
        done
    done
done


./waf --run "5g --bandwidth=$bandwidth --frequency=$frequency --iip=$interPacketInterval --speed=$speed --intraGroupDistance=$intraGroupDistance  --channel_condition=$environment1  --scenario=$commScenario2"
./waf --run "5g --bandwidth=$bandwidth --frequency=$frequency --iip=$interPacketInterval --speed=$speed --intraGroupDistance=$intraGroupDistance  --channel_condition=$environment1  --scenario=$commScenario3"
./waf --run "5g --bandwidth=$bandwidth --frequency=$frequency --iip=$interPacketInterval --speed=$speed --intraGroupDistance=$intraGroupDistance  --channel_condition=$environment1  --scenario=$commScenario4"
./waf --run "5g --bandwidth=$bandwidth --frequency=$frequency --iip=$interPacketInterval --speed=$speed --intraGroupDistance=$intraGroupDistance  --channel_condition=$environment2  --scenario=$commScenario1"
./waf --run "5g --bandwidth=$bandwidth --frequency=$frequency --iip=$interPacketInterval --speed=$speed --intraGroupDistance=$intraGroupDistance  --channel_condition=$environment2  --scenario=$commScenario2"
./waf --run "5g --bandwidth=$bandwidth --frequency=$frequency --iip=$interPacketInterval --speed=$speed --intraGroupDistance=$intraGroupDistance  --channel_condition=$environment2  --scenario=$commScenario3"
./waf --run "5g --bandwidth=$bandwidth --frequency=$frequency --iip=$interPacketInterval --speed=$speed --intraGroupDistance=$intraGroupDistance  --channel_condition=$environment2  --scenario=$commScenario4"
./waf --run "5g --bandwidth=$bandwidth --frequency=$frequency --iip=$interPacketInterval --speed=$speed --intraGroupDistance=$intraGroupDistance  --channel_condition=$environment3  --scenario=$commScenario1"
./waf --run "5g --bandwidth=$bandwidth --frequency=$frequency --iip=$interPacketInterval --speed=$speed --intraGroupDistance=$intraGroupDistance  --channel_condition=$environment3  --scenario=$commScenario2"
./waf --run "5g --bandwidth=$bandwidth --frequency=$frequency --iip=$interPacketInterval --speed=$speed --intraGroupDistance=$intraGroupDistance  --channel_condition=$environment3  --scenario=$commScenario3"
./waf --run "5g --bandwidth=$bandwidth --frequency=$frequency --iip=$interPacketInterval --speed=$speed --intraGroupDistance=$intraGroupDistance  --channel_condition=$environment3  --scenario=$commScenario4"
comment


