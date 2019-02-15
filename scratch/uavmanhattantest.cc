/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 *               2009,2010 Contributors
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
 * Author: Martín Giachino <martin.giachino@gmail.com>
 *
 *
 * This example demonstrates the use of Ns2MobilityHelper class to work with mobility.
 *
 * Detailed example description.
 *
 *  - intended usage: this should be used in order to load ns2 movement trace files into ns3.
 *  - behavior:
 *      - Ns2MobilityHelper object is created, associated to the specified trace file. 
 *      - A log file is created, using the log file name argument.
 *      - A node container is created with the number of nodes specified in the command line.  For the default ns-2 trace, specify the value 2 for this argument.
 *      - the program calls the Install() method of Ns2MobilityHelper to set mobility to nodes. At this moment, the file is read line by line, and the movement is scheduled in the simulator.
 *      - A callback is configured, so each time a node changes its course a log message is printed.
 *  - expected output: example prints out messages generated by each read line from the ns2 movement trace file.
 *                     For each line, it shows if the line is correct, or of it has errors and in this case it will
 *                     be ignored.
 *
 * Usage of ns2-mobility-trace:
 *
 *  ./waf --run "ns2-mobility-trace \
 *        --traceFile=src/mobility/examples/default.ns_movements
 *        --nodeNum=2  --duration=100.0 --logFile=ns2-mobility-trace.log"
 *
 *  NOTE: ns2-traces-file could be an absolute or relative path. You could use the file default.ns_movements
 *        included in the same directory as the example file.
 *  NOTE 2: Number of nodes present in the trace file must match with the command line argument.
 *          Note that you must know it before to be able to load it.
 *  NOTE 3: Duration must be a positive number and should match the trace file. Note that you must know it before to be able to load it.
 */


#include <iostream>
#include <fstream>
#include <sstream>

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/urbanuavmobility-module.h"
#include "ns3/ns2-mobility-helper.h"
#include "ns3/netanim-module.h"

using namespace ns3;

/*
// Prints actual position and velocity when a course change event occurs
static void
CourseChange (std::ostream *os, std::string foo, Ptr<const MobilityModel> mobility)
{
  Vector pos = mobility->GetPosition (); // Get position
  Vector vel = mobility->GetVelocity (); // Get velocity

  // Prints position and velocities
  *os << Simulator::Now () << " POS: x=" << pos.x << ", y=" << pos.y
      << ", z=" << pos.z << "; VEL:" << vel.x << ", y=" << vel.y
      << ", z=" << vel.z << std::endl;
}*/

// Example to use ns2 traces file in ns3
int main (int argc, char *argv[])
{
  std::string graphFile;
  std::string traceFile;
  std::string logFile;
  std::string animFile;

  int    nodeNum;
  double duration;
  double speed;
  double voltage;
  double capacity;

  // Enable logging from the ns2 helper
  LogComponentEnable ("Ns2MobilityHelper",LOG_LEVEL_DEBUG);

  // Parse command line attribute
  CommandLine cmd;
  cmd.AddValue ("graphFile", "Graph file", graphFile);
  cmd.AddValue ("traceFile", "Ns2 movement trace file", traceFile);
  cmd.AddValue ("nodeNum", "Number of nodes", nodeNum);
  cmd.AddValue ("duration", "Duration of Simulation", duration);
  cmd.AddValue ("logFile", "Log file", logFile);
  cmd.AddValue ("animFile", "Animation file", animFile);
  cmd.AddValue ("speed", "Speed", speed);
  cmd.AddValue ("voltage", "Voltage", voltage);
  cmd.AddValue ("capacity", "Capacity", capacity);
  cmd.Parse (argc,argv);

  // Check command line arguments
  if (graphFile.empty () || traceFile.empty () || nodeNum <= 0 || duration <= 0 || logFile.empty () || animFile.empty() || speed == 0 || voltage == 0 || capacity == 0 )
    {
      std::cout << "Usage of " << argv[0] << " :\n\n"
      "./waf --run \"mobilitytest"
      " --graphFile=randomgraph.xml"
      " --traceFile=src/mobility/examples/default.ns_movements"
      " --nodeNum=2 --duration=100.0 --logFile=mobility.log --animFile=mobility.xml --speed=10 --voltage=11.1 --capacity=1000\" \n\n"
      "NOTE: ns2-traces-file could be an absolute or relative path. You could use the file default.ns_movements\n"
      "      included in the same directory of this example file.\n\n"
      "NOTE 2: Number of nodes present in the trace file must match with the command line argument and must\n"
      "        be a positive number. Note that you must know it before to be able to load it.\n\n"
      "NOTE 3: Duration must be a positive number. Note that you must know it before to be able to load it.\n\n";

      return 0;
    }

  // Create Ns2MobilityHelper with the specified trace log file as parameter
  Ns2MobilityHelper ns2 = Ns2MobilityHelper (traceFile);

  // open log file for output
  std::ofstream os;
  os.open (logFile.c_str ());

  // Create all nodes.
  NodeContainer stas;
  stas.Create (nodeNum);

  ns2.Install (); // configure movements for each node, while reading trace file

  // UAVs
  UavContainer uavs;
  uavs.Create(1);
  Ptr<Uav> uav = uavs.Get(0);
  Ptr<UavEnergyModel> energyModel = CreateObject<UavEnergyModel>(uav,voltage,capacity);
  Ptr<UavManhattanMobilityModel> mobilityModel = CreateObject<UavManhattanMobilityModel>((char*)graphFile.c_str(),energyModel,speed,"random");
  uav->AggregateObject(mobilityModel);

  Simulator::Stop (Seconds (duration));

  AnimationInterface anim (animFile);
  anim.UpdateNodeSize(nodeNum,10,10);
  Simulator::Run ();
  Simulator::Destroy ();

  mobilityModel->printTrajectory();
  mobilityModel->printCoverage(10,10);
  os.close (); // close log file
  return 0;
}