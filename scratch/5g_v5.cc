/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
*   Copyright (c) 2020 University of Padova, Dep. of Information Engineering,
*   SIGNET lab.
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License version 2 as
*   published by the Free Software Foundation;
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "ns3/mmwave-sidelink-spectrum-phy.h"
#include "ns3/mmwave-vehicular-net-device.h"
#include "ns3/mmwave-vehicular-helper.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/mobility-module.h"
#include "ns3/config.h"
#include "ns3/isotropic-antenna-model.h"
#include "ns3/spectrum-helper.h"
#include "ns3/mmwave-spectrum-value-helper.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/core-module.h"
#include "ns3/mmwave-vehicular-propagation-loss-model.h"


// include and usings for output results into csv file, config GUI invoked
#include <iostream>
#include <fstream>
#include <cstdlib> // for exit function
using std::ofstream;
using std::cerr;
using std::endl;

#include "ns3/config-store-module.h"

NS_LOG_COMPONENT_DEFINE ("5G");

using namespace ns3;
using namespace millicar;


uint32_t g_rxPackets; // total number of received packets
uint32_t g_txPackets; // total number of transmitted packets

Time g_firstReceived; // timestamp of the first time a packet is received
Time g_lastReceived; // timestamp of the last received packet


double humidity;
double txPower=15;

void MmWaveProp(double humidity, double stepTime){

Ptr<MmWaveVehicularPropagationLossModel> propagationLossModel = CreateObject<MmWaveVehicularPropagationLossModel> ();

propagationLossModel->SetHumidity(humidity);

/*
auto channel = DynamicCast<MmWaveVehicularNetDevice>(devs.Get(0))->GetPhy()->GetSpectrumPhy()->GetSpectrumChannel();
  PointerValue plm;
  channel->GetAttribute("PropagationLossModel", plm);
  Ptr<MmWaveVehicularPropagationLossModel> pathloss = DynamicCast<MmWaveVehicularPropagationLossModel>(plm.Get<PropagationLossModel>());

  pathloss->SetHumidity(humidity);

*/   


std::cout << "Humidity:\t" << humidity << std::endl;


humidity +=1;

if (humidity >=100) {
       humidity=100;  

    }

Simulator::Schedule(Seconds(stepTime), &MmWaveProp, humidity, stepTime);

}


void computeRxPower(NetDeviceContainer devs, double stepTime) {
  auto channel = DynamicCast<MmWaveVehicularNetDevice>(devs.Get(0))->GetPhy()->GetSpectrumPhy()->GetSpectrumChannel();
  PointerValue plm;
  channel->GetAttribute("PropagationLossModel", plm);
  Ptr<MmWaveVehicularPropagationLossModel> pathloss = DynamicCast<MmWaveVehicularPropagationLossModel>(plm.Get<PropagationLossModel>());

  Ptr<MobilityModel> mobileNode0 = devs.Get(0)->GetNode()->GetObject<MobilityModel>();
  Ptr<MobilityModel> mobileNode1 = devs.Get(1)->GetNode()->GetObject<MobilityModel>();
  

  double RxPowerVal = pathloss ->DoCalcRxPower( txPower, mobileNode1, mobileNode0);

  //double PL = pathloss -> GetLoss (mobileNode1, mobileNode0);

  std::cout << "\n The value of the RxPOWER is: " << RxPowerVal << std::endl;

 // std::cout << "\n The value of the PathLoss is: " << PL << std::endl;

  std::ofstream outdata; // outdata is like cin
  
  outdata.open("RxPower5.91k.csv", std::ofstream::app); // opens the file
   if( !outdata ) { // file couldn't be opened
      cerr << "Error: file could not be opened" << endl;
      exit(1);
   }
  outdata << RxPowerVal<< endl;
 // outdata << PL<< endl;
  outdata.close();

 
  Simulator::Schedule(Seconds(stepTime), &computeRxPower, devs, stepTime);
}
 
/*
static void CalRxPower(Ptr< MobilityModel > mob1, Ptr< MobilityModel > mob2){

Ptr<MmWaveVehicularPropagationLossModel> propagationLossModel = CreateObject<MmWaveVehicularPropagationLossModel> ();
double RxPower= propagationLossModel-> DoCalcRxPower( 15, mob1, mob2);

std::cout << "RxPower:\t" << RxPower << std::endl;

  std::ofstream outdata; // outdata is like cin
  
  outdata.open("RxPower.csv", std::ofstream::app); // opens the file
   if( !outdata ) { // file couldn't be opened
      cerr << "Error: file could not be opened" << endl;
      exit(1);
   }
  outdata << RxPower<< endl;
  outdata.close();

Simulator::Schedule (Seconds (2), &CalRxPower,mob1, mob2);


}*/

static void Rx (Ptr<OutputStreamWrapper> stream, Ptr<const Packet> p)
{
 g_rxPackets++;
 
 SeqTsHeader header;

 p->PeekHeader(header);

 *stream->GetStream () << Simulator::Now ().GetSeconds () << "\t" << p->GetSize() << "\t" << header.GetSeq() << "\t" << header.GetTs().GetSeconds() << std::endl;

 if (g_rxPackets > 1)
 {

   g_lastReceived = Simulator::Now();
 }
 else
 {
   g_firstReceived = Simulator::Now();
 }
}

int main (int argc, char *argv[])
{
  // This script creates two nodes moving at 20 m/s, placed at a distance of intraGroupDistance m.
  // These nodes exchange packets through a UDP application,
  // and they communicate using a wireless channel.
  // This V1 use RxPower to check receiver's power in dBm

  // system parameters
  double bandwidth ; // bandwidth in Hz
  double frequency ; // the carrier frequency
  uint32_t numerology=3 ; // the numerology

  // applications
  uint32_t packetSize = 1024; // UDP packet size in bytes
  uint32_t startTime = 0.05; // application start time in milliseconds
  uint32_t endTime = 100; // application end time in milliseconds
  
  uint32_t interPacketInterval; // interpacket interval in microseconds

  // mobility
  double speed; // speed of the vehicles m/s
  double intraGroupDistance ; // distance between two vehicles belonging to the same group

  //three weather parameters
  double particleradius;
  double visibility;
  double humidity;

  // channel and scenario choose
  std::string channel_condition;
  std::string scenario;

  double stepTime = 0.001;
  double stepTime2= 0.002;

  CommandLine cmd;
  //
  cmd.AddValue ("bandwidth", "used bandwidth", bandwidth);
  cmd.AddValue ("frequency", "set the carrier frequency", frequency);
  cmd.AddValue ("numerology", "set the numerology to use at the physical layer", numerology);
  cmd.AddValue ("iip", "inter packet interval, in microseconds", interPacketInterval);
  
      
  // cmd new add values
  cmd.AddValue ("speed", "set vehicle speed", speed);
  cmd.AddValue ("intraGroupDistance", "distance between two vehicles belonging to the same group, y-coord", intraGroupDistance);
  cmd.AddValue("channel_condition", "Defines the channel condition", channel_condition);
  cmd.AddValue("scenario", "Defines the scenario where the communication takes place", scenario);

  //cmd add weather impacts
  cmd.AddValue("Pvalue", "Particle radius value", particleradius);
  cmd.AddValue("Vvalue", "Visibility value", visibility);
  cmd.AddValue("Hvalue", "H valueeee", humidity);

  cmd.Parse (argc, argv);

  Config::SetDefault ("ns3::MmWaveSidelinkMac::UseAmc", BooleanValue (true));
  Config::SetDefault ("ns3::MmWavePhyMacCommon::CenterFreq", DoubleValue (frequency));
  Config::SetDefault ("ns3::MmWaveVehicularHelper::Bandwidth", DoubleValue (bandwidth));
  Config::SetDefault ("ns3::MmWaveVehicularHelper::Numerology", UintegerValue (numerology));
  
  // set channelcondition and scenario
  Config::SetDefault ("ns3::MmWaveVehicularPropagationLossModel::ChannelCondition", StringValue (channel_condition));
  Config::SetDefault("ns3::MmWaveVehicularPropagationLossModel::Scenario", StringValue(scenario));

  // set weather impacts + frequency in MmWaveVehicularPropagationLossModel
  Config::SetDefault("ns3::MmWaveVehicularPropagationLossModel::ParticleRadius", DoubleValue(particleradius));
  Config::SetDefault("ns3::MmWaveVehicularPropagationLossModel::VVisibility", DoubleValue(visibility));
  //Config::SetDefault("ns3::MmWaveVehicularPropagationLossModel::HHumidity", DoubleValue(humidity));
  
  
  Config::SetDefault("ns3::MmWaveVehicularPropagationLossModel::Frequency", DoubleValue(frequency));


  Config::SetDefault ("ns3::MmWaveVehicularPropagationLossModel::Shadowing", BooleanValue (true));
  Config::SetDefault ("ns3::MmWaveVehicularSpectrumPropagationLossModel::UpdatePeriod", TimeValue (MilliSeconds (1)));
  Config::SetDefault ("ns3::MmWaveVehicularAntennaArrayModel::AntennaElements", UintegerValue (16));
  Config::SetDefault ("ns3::MmWaveVehicularAntennaArrayModel::AntennaElementPattern", StringValue ("3GPP-V2V"));
  Config::SetDefault ("ns3::MmWaveVehicularAntennaArrayModel::IsotropicAntennaElements", BooleanValue (true));
  Config::SetDefault ("ns3::MmWaveVehicularAntennaArrayModel::NumSectors", UintegerValue (2));
  
  Config::SetDefault ("ns3::MmWaveVehicularNetDevice::RlcType", StringValue("LteRlcUm"));
  Config::SetDefault ("ns3::MmWaveVehicularHelper::SchedulingPatternOption", EnumValue(2)); // use 2 for SchedulingPatternOption=OPTIMIZED, 1 or SchedulingPatternOption=DEFAULT
  Config::SetDefault ("ns3::LteRlcUm::MaxTxBufferSize", UintegerValue (500*1024));


  // create the nodes
  NodeContainer n;
  n.Create (2);
  
  // create the mobility models
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobility.Install (n);

  n.Get (0)->GetObject<MobilityModel> ()->SetPosition (Vector (0,0,0));
  n.Get (0)->GetObject<ConstantVelocityMobilityModel> ()->SetVelocity (Vector (0, speed, 0));

  n.Get (1)->GetObject<MobilityModel> ()->SetPosition (Vector (0, intraGroupDistance,  0));
  n.Get (1)->GetObject<ConstantVelocityMobilityModel> ()->SetVelocity (Vector (0, speed, 0));

  // create and configure the helper
  Ptr<MmWaveVehicularHelper> helper = CreateObject<MmWaveVehicularHelper> ();
  helper->SetNumerology (3);
  helper->SetPropagationLossModelType ("ns3::MmWaveVehicularPropagationLossModel");
  helper->SetSpectrumPropagationLossModelType ("ns3::MmWaveVehicularSpectrumPropagationLossModel");
  NetDeviceContainer devs = helper->InstallMmWaveVehicularNetDevices (n);
  

  // increase humidity by 1% every 1 second
/*  for (int t =0; t<100; t++){
  
  Simulator::Schedule(Seconds(1*t), &MmWaveProp, 1*t);
  
 
  }
  
 
  // calculate RxPower every 2 second
  
  Time delay = MilliSeconds (2000);   
  Simulator::Schedule (delay, &CalRxPower,n.Get (0)->GetObject<MobilityModel> (), n.Get (1)->GetObject<MobilityModel> ());*/
  
  // increase humidity by 1% every 1 second
  //for (int t =0; t<20; t++){
  
        //Simulator::Schedule(Seconds(1*t), &MmWaveProp, 5*t);

  // calculate RxPower every 2 second

    //if ( t % 5 == 0){
            //    Simulator::Schedule (Seconds(1*t), &CalRxPower,n.Get (0)->GetObject<MobilityModel> (), n.Get (1)->GetObject<MobilityModel> ());

    //  }
// }
  


  // Install the TCP/IP stack in the two nodes
  InternetStackHelper internet;
  internet.Install (n);

  Ipv4AddressHelper ipv4;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (devs);

  // Need to pair the devices in order to create a correspondence between transmitter and receiver
  // and to populate the < IP addr, RNTI > map.
  helper->PairDevices(devs);


  // Set the routing table
  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> staticRouting = ipv4RoutingHelper.GetStaticRouting (n.Get (0)->GetObject<Ipv4> ());
  staticRouting->SetDefaultRoute (n.Get (1)->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal () , 2 );

  NS_LOG_DEBUG("IPv4 Address node 0: " << n.Get (0)->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ());
  NS_LOG_DEBUG("IPv4 Address node 1: " << n.Get (1)->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal ());

  Ptr<mmwave::MmWaveAmc> m_amc = CreateObject <mmwave::MmWaveAmc> (helper->GetConfigurationParameters());

  // setup the applications
  Config::SetDefault ("ns3::UdpClient::MaxPackets", UintegerValue (0xFFFFFFFF));
  Config::SetDefault ("ns3::UdpClient::Interval", TimeValue (MicroSeconds (interPacketInterval)));
  Config::SetDefault ("ns3::UdpClient::PacketSize", UintegerValue (packetSize));

  // create the applications
  uint32_t port = 4000;

  UdpEchoServerHelper server (port);
  
  ApplicationContainer echoApps = server.Install (n.Get (1));
  echoApps.Start (Seconds (0.0));


  AsciiTraceHelper asciiTraceHelper;
  Ptr<OutputStreamWrapper> stream = asciiTraceHelper.CreateFileStream ("simple-one-stats.txt");
  echoApps.Get(0)->TraceConnectWithoutContext ("Rx", MakeBoundCallback (&Rx, stream));


  UdpClientHelper client (n.Get (1)->GetObject<Ipv4> ()->GetAddress (1, 0).GetLocal (), port);
  ApplicationContainer apps = client.Install (n.Get (0));
  

  // set the application start/end time
  apps.Start (MilliSeconds (startTime));
  apps.Stop (MilliSeconds (endTime));
  
  // Invoke GUI just before entering Simulator::Run 
  /*GtkConfigStore config;
  config.ConfigureDefaults ();
  config.ConfigureAttributes ();*/
  
  MmWaveProp(humidity, stepTime);
  //MmWaveProp(devs,humidity, stepTime);
  computeRxPower(devs, stepTime2);
  Simulator::Stop (MilliSeconds (endTime + 1));
   
    
  Simulator::Run ();
  Simulator::Destroy ();

  double throughput = (double(g_rxPackets)*(double(packetSize)*8)/double( g_lastReceived.GetSeconds() - g_firstReceived.GetSeconds()))/1e6;
  
  std::cout << "----------- Statistics -----------" << std::endl;
  std::cout << "Packets size:\t\t" << packetSize << " Bytes" << std::endl;
  
  std::cout << "Packets received:\t" << g_rxPackets << std::endl;
  std::cout << "Average Throughput:\t" << throughput << " Mbps" << std::endl;
  std::cout << "First Received Time:\t" << g_firstReceived.GetSeconds() << std::endl;
  std::cout << "Last Received Time:\t" << g_lastReceived.GetSeconds() << std::endl;

  // create a result.csv file under ns3 default folder and output received packets, throughput, particle radius,visibility,humidity, frequency,
  // first and last packet received time, intraGroupDistance into the file.
/*  
  std::ofstream outdata; // outdata is like cin
  
  outdata.open("result5g_v1.csv", std::ofstream::app); // opens the file
   if( !outdata ) { // file couldn't be opened
      cerr << "Error: file could not be opened" << endl;
      exit(1);
   }
  outdata << g_rxPackets<<","<<throughput<<","<<RxPower<<","<<particleradius<<","<<visibility<<","<<humidity<<","<<frequency<<","<<g_firstReceived.GetSeconds()<<","<<g_lastReceived.GetSeconds()<<","<<intraGroupDistance << endl;
  outdata.close();
  */

  return 0;
}
