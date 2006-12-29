/**
 * @file  rastest.cpp
 * @brief REPLACE_WITH_ONE_LINE_SHORT_DESCRIPTION
 *
 * REPLACE_WITH_LONG_DESCRIPTION_OR_REFERENCE
 */
/*
 * Original Author: REPLACE_WITH_FULL_NAME_OF_CREATING_AUTHOR 
 * CVS Revision Info:
 *    $Author: nicks $
 *    $Date: 2006/12/29 01:49:46 $
 *    $Revision: 1.2 $
 *
 * Copyright (C) 2002-2007,
 * The General Hospital Corporation (Boston, MA). 
 * All rights reserved.
 *
 * Distribution, usage and copying of this software is covered under the
 * terms found in the License Agreement file named 'COPYING' found in the
 * FreeSurfer source code root directory, and duplicated here:
 * https://surfer.nmr.mgh.harvard.edu/fswiki/FreeSurferOpenSourceLicense
 *
 * General inquiries: freesurfer@nmr.mgh.harvard.edu
 * Bug reports: analysis-bugs@nmr.mgh.harvard.edu
 *
 */


//
// rastest.cpp
//

#include <iostream>
#include <iomanip>

extern "C"
{
#include "mri.h"
  char *Progname="rastest";
}

using namespace std;

double t = 0.000001;

bool equal(double &x, double &y, double &tol)
{
  if (fabs(x-y) < tol)
    return true;
  else
    return false;
}
int main(int argc, char *argv[])
{
  // verify various vox, ras, surfaceRAS, relations
  if (argc < 2)
  {
    cout << "Usage: rastest <vol>" << endl;
    exit(1);
  }
  MRI *mri = MRIread(argv[1]);
  double xr, yr, zr, xr1, yr1, zr1;
  double xsr, ysr, zsr, xsr1, ysr1, zsr1;
  double xv, yv, zv, xv1, yv1, zv1, xv2, yv2, zv2;
  char more = 'y';
  while (more != 'n')
  {
    cout << "vox x: ";
    cin >> xv;
    cout << "vox y: ";
    cin >> yv;
    cout << "vox z: ";
    cin >> zv;
    cout << "Going from Voxel to SurfaceRAS " << endl;
    // one way to get to surface RAS
    MRIvoxelToWorld(mri, xv, yv, zv, &xr, &yr, &zr);
    MRIRASToSurfaceRAS(mri, xr, yr, zr, &xsr, &ysr, &zsr);
    // another way to get to surface RAS
    MRIvoxelToSurfaceRAS(mri, xv, yv, zv, &xsr1, &ysr1, &zsr1);
    // compare
    if (!equal(xsr, xsr1, t) || !equal(ysr, ysr1, t) || !equal(zsr, zsr1, t))
      cout << "Error" << endl;
    else
      cout << "Fine" << endl;
    cout << '\t' << xsr << " <-> " << xsr1 << endl;
    cout << '\t' << ysr << " <-> " << ysr1 << endl;
    cout << '\t' << zsr << " <-> " << zsr1 << endl;
    //////////////////////////////////////////////////////////////
    cout << "Coming back from SurfaceRAS to Voxel " << endl;
    // one way
    MRIsurfaceRASToRAS(mri, xsr, ysr, zsr, &xr1, &yr1, &zr1);
    MRIworldToVoxel(mri, xr1, yr1, zr1, &xv1, &yv1, &zv1);
    // another way
    MRIsurfaceRASToVoxel(mri, xsr, ysr, zsr, &xv2, &yv2, &zv2);
    // compare
    if (!equal(xv1, xv2, t) || !equal(yv1, yv2, t) || !equal(zv1, zv2, t))
      cout << "Error *****************************" << endl;
    else
      cout << "Fine" << endl;
    cout << '\t' << xv1 << " <-> " << xv2 << endl;
    cout << '\t' << yv1 << " <-> " << yv2 << endl;
    cout << '\t' << zv1 << " <-> " << zv2 << endl;

    cout << "RAS location comparision going and coming back" << endl;
    // RAS comparison
    if (!equal(xr, xr1, t) || !equal(yr, yr1, t) || !equal(zr, zr1, t))
      cout << "Error ****************************" << endl;
    else
      cout << "Fine" << endl;
    cout << '\t' << xr << " <-> " << xr1 << endl;
    cout << '\t' << yr << " <-> " << yr1 << endl;
    cout << '\t' << zr << " <-> " << zr1 << endl;

    cout << "More? (y/n) ";
    cin >> more;
  }
}
