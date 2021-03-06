/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2018 Computer Science Department, FAST-NU, Lahore.
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
 * Author: Farooq Ahmed <farooq.ahmad@nu.edu.pk>
 */
#ifndef UAV_MANHATTAN_H
#define UAV_MANHATTAN_H

#include "graph-mobility-model.h"
#include <vector>
#include "ns3/vector.h"

using namespace std;

namespace ns3 {

/**
 * \ingroup mobility
 * \brief UAV Manhattan mobility model.
 *
 */
class UavManhattanMobilityModel : public GraphMobilityModel
{
protected:
  vector<Vector> trajectory;
  Ptr<UavEnergyModel> energyModel;

  void move();
  void halt();
  virtual void pause();
  virtual void resume();
  void recordTrajectory();
 
public:

  static TypeId GetTypeId (void);
  UavManhattanMobilityModel ();
  UavManhattanMobilityModel(char* file,Ptr<UavEnergyModel> energy,double maxSpeed,string selectionStrategy = "random");
  void printTrajectory();
  void printCoverage(float,float);

};

} // namespace ns3

#endif /* UAV_MANHATTAN_MODEL_H */

