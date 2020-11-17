/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Universita' di Firenze, Italy
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Tommaso Pecorella <tommaso.pecorella@unifi.it>
 */

// Network topology
//
//    SRC
//     |<=== source network
//     A-----B
//      \   / \   all networks have cost 1, except
//       \ /  |   for the direct link from C to D, which
//        C  /    has cost 10
//        | /
//        |/
//        D
//        |<=== target network
//       DST
//
//
// A, B, C and D are RIPng routers.
// A and D are configured with static addresses.
// SRC and DST will exchange packets.
//
// After about 3 seconds, the topology is built, and Echo Reply will be received.
// After 40 seconds, the link between B and D will break, causing a route failure.
// After 44 seconds from the failure, the routers will recovery from the failure.
// Split Horizoning should affect the recovery time, but it is not. See the manual
// for an explanation of this effect.
//
// If "showPings" is enabled, the user will see:
// 1) if the ping has been acknowledged
// 2) if a Destination Unreachable has been received by the sender
// 3) nothing, when the Echo Request has been received by the destination but
//    the Echo Reply is unable to reach the sender.
// Examining the .pcap files with Wireshark can confirm this effect.


#include <fstream>
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-routing-table-entry.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("RipSimpleRouting");

void TearDownLink (Ptr<Node> nodeA, Ptr<Node> nodeB, uint32_t interfaceA, uint32_t interfaceB)
{
  nodeA->GetObject<Ipv4> ()->SetDown (interfaceA);
  nodeB->GetObject<Ipv4> ()->SetDown (interfaceB);
}

int main (int argc, char **argv)
{
  bool verbose = false;
  bool printRoutingTables = false;
  bool showPings = false;
  std::string SplitHorizon ("PoisonReverse");

  CommandLine cmd;
  cmd.AddValue ("verbose", "turn on log components", verbose);
  cmd.AddValue ("printRoutingTables", "Print routing tables at 30, 60 and 90 seconds", printRoutingTables);
  cmd.AddValue ("showPings", "Show Ping6 reception", showPings);
  cmd.AddValue ("splitHorizonStrategy", "Split Horizon strategy to use (NoSplitHorizon, SplitHorizon, PoisonReverse)", SplitHorizon);
  cmd.Parse (argc, argv);

  if (verbose)
    {
      LogComponentEnableAll (LogLevel (LOG_PREFIX_TIME | LOG_PREFIX_NODE));
      LogComponentEnable ("RipSimpleRouting", LOG_LEVEL_INFO);
      LogComponentEnable ("Rip", LOG_LEVEL_ALL);
      LogComponentEnable ("Ipv4Interface", LOG_LEVEL_ALL);
      LogComponentEnable ("Icmpv4L4Protocol", LOG_LEVEL_ALL);
      LogComponentEnable ("Ipv4L3Protocol", LOG_LEVEL_ALL);
      LogComponentEnable ("ArpCache", LOG_LEVEL_ALL);
      LogComponentEnable ("V4Ping", LOG_LEVEL_ALL);
    }

  if (SplitHorizon == "NoSplitHorizon")
    {
      Config::SetDefault ("ns3::Rip::SplitHorizon", EnumValue (RipNg::NO_SPLIT_HORIZON));
    }
  else if (SplitHorizon == "SplitHorizon")
    {
      Config::SetDefault ("ns3::Rip::SplitHorizon", EnumValue (RipNg::SPLIT_HORIZON));
    }
  else
    {
      Config::SetDefault ("ns3::Rip::SplitHorizon", EnumValue (RipNg::POISON_REVERSE));
    }

  NS_LOG_INFO ("Create nodes.");
  Ptr<Node> a = CreateObject<Node> ();
 Names::Add ("ANode", a);
 Ptr<Node> b = CreateObject<Node> ();
 Names::Add ("BNode", b);
 Ptr<Node> c = CreateObject<Node> ();
 Names::Add ("CNode", c);
 Ptr<Node> d = CreateObject<Node> ();
 Names::Add ("DNode", d);
 Ptr<Node> e = CreateObject<Node> ();
 Names::Add ("ENode", e);
 Ptr<Node> f = CreateObject<Node> ();
 Names::Add ("FNode", f);
 Ptr<Node> g = CreateObject<Node> ();
 Names::Add ("GNode", g);
 Ptr<Node> h = CreateObject<Node> ();
 Names::Add ("HNode", h);
 Ptr<Node> i = CreateObject<Node> ();
 Names::Add ("INode", i);
 Ptr<Node> j = CreateObject<Node> ();
 Names::Add ("JNode", j);
 Ptr<Node> k = CreateObject<Node> ();
 Names::Add ("KNode", k);
 Ptr<Node> ro1 = CreateObject<Node> ();
 Names::Add ("Routerro1", ro1);
 Ptr<Node> ro2 = CreateObject<Node> ();
 Names::Add ("Routerro2", ro2);
 Ptr<Node> ro3 = CreateObject<Node> ();
 Names::Add ("Routerro3", ro3);
 Ptr<Node> ro4 = CreateObject<Node> ();
 Names::Add ("Routerro4", ro4);
 Ptr<Node> ro5 = CreateObject<Node> ();
 Names::Add ("Routerro5", ro5);
 Ptr<Node> ro6 = CreateObject<Node> ();
 Names::Add ("Routerro6", ro6);
  NodeContainer n1 (a, ro1);
   NodeContainer n2 (b, ro1);
   NodeContainer n3 (c, ro1);
   NodeContainer n4 (d, ro2);
   NodeContainer n5 (e, ro2);
   NodeContainer n6 (f, ro2);
   NodeContainer n7 (g, ro2);
   NodeContainer n8 (ro1, ro3);
   NodeContainer n9 (ro2, ro4);
   NodeContainer n10 (ro3, ro4);
   NodeContainer n11 (ro3, ro5);
   NodeContainer n12 (ro4, ro5);
   NodeContainer n13 (ro4, ro6);
   NodeContainer n14 (ro5, h);
   NodeContainer n15 (ro5, i);
   NodeContainer n16 (ro6, j);
   NodeContainer n17 (ro6, k);
   NodeContainer routers (ro1,ro2,ro3,ro4,ro5,ro6);
   NodeContainer nodes (a,b,c,d,e,f,g,h,i,j);
 

  NS_LOG_INFO ("Create channels.");
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (5000000));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  NetDeviceContainer ndc1 = csma.Install (n1);
  NetDeviceContainer ndc2 = csma.Install (n2);
  NetDeviceContainer ndc3 = csma.Install (n3);
  NetDeviceContainer ndc4 = csma.Install (n4);
  NetDeviceContainer ndc5 = csma.Install (n5);
  NetDeviceContainer ndc6 = csma.Install (n6);
  NetDeviceContainer ndc7 = csma.Install (n7);
  NetDeviceContainer ndc8 = csma.Install (n8);
  NetDeviceContainer ndc9 = csma.Install (n9);
  NetDeviceContainer ndc10 = csma.Install (n10);
  NetDeviceContainer ndc11 = csma.Install (n11);
  NetDeviceContainer ndc12 = csma.Install (n12);
  NetDeviceContainer ndc13 = csma.Install (n13);
  NetDeviceContainer ndc14 = csma.Install (n14);
  NetDeviceContainer ndc15 = csma.Install (n15);
  NetDeviceContainer ndc16 = csma.Install (n16);
  NetDeviceContainer ndc17 = csma.Install (n17);

  NS_LOG_INFO ("Create IPv4 and routing");
  RipHelper ripRouting;

  // Rule of thumb:
  // Interfaces are added sequentially, starting from 0
  // However, interface 0 is always the loopback...
  ripRouting.ExcludeInterface (ro1, 1);
   ripRouting.ExcludeInterface (ro1, 2);
   ripRouting.ExcludeInterface (ro1, 3);
   ripRouting.ExcludeInterface (ro2, 1);
   ripRouting.ExcludeInterface (ro2, 2);
   ripRouting.ExcludeInterface (ro2, 3);
   ripRouting.ExcludeInterface (ro2, 4);
   ripRouting.ExcludeInterface (ro5, 3);
   ripRouting.ExcludeInterface (ro5, 4);
   ripRouting.ExcludeInterface (ro6, 2);
   ripRouting.ExcludeInterface (ro6, 3);

  ripRouting.SetInterfaceMetric (ro5, 2, 5);
    ripRouting.SetInterfaceMetric (ro4, 3, 5);

  Ipv4ListRoutingHelper listRH;
  listRH.Add (ripRouting, 0);
//  Ipv4StaticRoutingHelper staticRh;
//  listRH.Add (staticRh, 5);

  InternetStackHelper internet;
  internet.SetIpv6StackInstall (false);
  internet.SetRoutingHelper (listRH);
  internet.Install (routers);

  InternetStackHelper internetNodes;
  internetNodes.SetIpv6StackInstall (false);
  internetNodes.Install (nodes);

  // Assign addresses.
  // The source and destination networks have global addresses
  // The "core" network just needs link-local addresses for routing.
  // We assign global addresses to the routers as well to receive
  // ICMPv6 errors.
  NS_LOG_INFO ("Assign IPv4 Addresses.");
  Ipv4AddressHelper ipv4;

  ipv4.SetBase (Ipv4Address ("10.0.0.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic1 = ipv4.Assign (ndc1);

  ipv4.SetBase (Ipv4Address ("10.0.1.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic2 = ipv4.Assign (ndc2);

  ipv4.SetBase (Ipv4Address ("10.0.2.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic3 = ipv4.Assign (ndc3);

  ipv4.SetBase (Ipv4Address ("10.0.3.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic4 = ipv4.Assign (ndc4);

  ipv4.SetBase (Ipv4Address ("10.0.4.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic5 = ipv4.Assign (ndc5);

  ipv4.SetBase (Ipv4Address ("10.0.5.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic6 = ipv4.Assign (ndc6);

  ipv4.SetBase (Ipv4Address ("10.0.6.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic7 = ipv4.Assign (ndc7);
ipv4.SetBase (Ipv4Address ("10.0.7.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic7 = ipv4.Assign (ndc8);
ipv4.SetBase (Ipv4Address ("10.0.8.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic7 = ipv4.Assign (ndc9);
ipv4.SetBase (Ipv4Address ("10.0.10.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic7 = ipv4.Assign (ndc11);
ipv4.SetBase (Ipv4Address ("10.0.9.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic7 = ipv4.Assign (ndc10);
ipv4.SetBase (Ipv4Address ("10.0.11.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic7 = ipv4.Assign (ndc12);
ipv4.SetBase (Ipv4Address ("10.0.12.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic7 = ipv4.Assign (ndc13);
ipv4.SetBase (Ipv4Address ("10.0.13.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic7 = ipv4.Assign (ndc14);
ipv4.SetBase (Ipv4Address ("10.0.14.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic7 = ipv4.Assign (ndc15);
ipv4.SetBase (Ipv4Address ("10.0.15.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic7 = ipv4.Assign (ndc16);
ipv4.SetBase (Ipv4Address ("10.0.16.0"), Ipv4Mask ("255.255.255.0"));
  Ipv4InterfaceContainer iic7 = ipv4.Assign (ndc17);

  Ptr<Ipv4StaticRouting> staticRouting;
  

   staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (k->GetObject<Ipv4> ()->GetRoutingProtocol ());
   staticRouting->SetDefaultRoute ("10.0.16.1", 1 );
   staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (j->GetObject<Ipv4> ()->GetRoutingProtocol ());
   staticRouting->SetDefaultRoute ("10.0.15.1", 1 );
   staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (i->GetObject<Ipv4> ()->GetRoutingProtocol ());
   staticRouting->SetDefaultRoute ("10.0.14.1", 1 );
   staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (h->GetObject<Ipv4> ()->GetRoutingProtocol ());
   staticRouting->SetDefaultRoute ("10.0.13.1", 1 );


   staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (g->GetObject<Ipv4> ()->GetRoutingProtocol ());
   staticRouting->SetDefaultRoute ("10.0.6.2", 1 );
   staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (f->GetObject<Ipv4> ()->GetRoutingProtocol ());
   staticRouting->SetDefaultRoute ("10.0.5.2", 1 );
   staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (e->GetObject<Ipv4> ()->GetRoutingProtocol ());
   staticRouting->SetDefaultRoute ("10.0.4.2", 1 );
   staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (d->GetObject<Ipv4> ()->GetRoutingProtocol ());
   staticRouting->SetDefaultRoute ("10.0.3.2", 1 );
   
   

  staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (c->GetObject<Ipv4> ()->GetRoutingProtocol ());
   staticRouting->SetDefaultRoute ("10.0.2.2", 1 );
  staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (b->GetObject<Ipv4> ()->GetRoutingProtocol ());
   staticRouting->SetDefaultRoute ("10.0.1.2", 1 );
   staticRouting = Ipv4RoutingHelper::GetRouting <Ipv4StaticRouting> (src->GetObject<Ipv4> ()->GetRoutingProtocol ());
  staticRouting->SetDefaultRoute ("10.0.0.2", 1 );
   

  if (printRoutingTables)
    {
      RipHelper routingHelper;

      Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> (&std::cout);

      routingHelper.PrintRoutingTableAt (Seconds (30.0), ro1, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (30.0), ro2, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (30.0), ro3, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (30.0), ro4, routingStream);

      routingHelper.PrintRoutingTableAt (Seconds (60.0), ro1, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (60.0), ro2, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (60.0), ro3, routingStream);
      routingHelper.PrintRoutingTableAt (Seconds (60.0), ro4, routingStream);

    }

  NS_LOG_INFO ("Create Applications.");
  uint32_t packetSize = 1024;
  Time interPacketInterval = Seconds (1.0);
  V4PingHelper ping ("10.0.16.2");

  ping.SetAttribute ("Interval", TimeValue (interPacketInterval));
  ping.SetAttribute ("Size", UintegerValue (packetSize));
  if (showPings)
    {
      ping.SetAttribute ("Verbose", BooleanValue (true));
    }
  ApplicationContainer apps = ping.Install (a);
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (80.0));

  V4PingHelper ping22 ("10.0.13.2");

  ping22.SetAttribute ("Interval", TimeValue (interPacketInterval));
  ping22.SetAttribute ("Size", UintegerValue (packetSize));
  if (showPings)
    {
      ping22.SetAttribute ("Verbose", BooleanValue (true));
    }
  ApplicationContainer apps1 = ping22.Install (a);
  apps1.Start (Seconds (1.0));
  apps1.Stop (Seconds (80.0));


  AsciiTraceHelper ascii;
  csma.EnableAsciiAll (ascii.CreateFileStream ("rip-simple-routing.tr"));
  csma.EnablePcapAll ("rip-simple-routing", true);

  Simulator::Schedule (Seconds (25), &TearDownLink, ro3, ro4, 2, 2);

  /* Now, do the actual simulation. */
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds (90.0));
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}

