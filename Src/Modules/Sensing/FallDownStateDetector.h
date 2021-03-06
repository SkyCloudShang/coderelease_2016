/**
 * @file FallDownStateDetector.h
 *
 * This file declares a module that provides information about the current state of the robot's body.
 *
 * @author <a href="mailto:maring@informatik.uni-bremen.de">Martin Ring</a>
 */

#pragma once

#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/MotionControl/MotionInfo.h"
#include "Representations/Sensing/FallDownState.h"
#include "Representations/Sensing/InertialData.h"
#include "Tools/Math/Angle.h"
#include "Tools/Module/Module.h"
#include "Tools/RingBufferWithSum.h"

MODULE(FallDownStateDetector,
{,
  REQUIRES(FrameInfo),
  REQUIRES(InertialData),
  USES(MotionInfo),
  PROVIDES(FallDownState),
  LOADS_PARAMETERS(
  {,
    (int) fallTime, /**< The time (in ms) to remain in state 'falling' after a detected fall */
    (Angle) staggeringAngleX, /**< The threshold angle which is used to detect the robot is staggering sidewards */
    (Angle) staggeringAngleY, /**< The threshold angle which is used to detect the robot is staggering to the back or front */
    (Angle) fallDownAngleX, /**< The threshold angle which is used to detect a sidewards fall */
    (Angle) fallDownAngleY, /**< The threshold angle which is used to detect a fall to the back or front */
    (Angle) onGroundAngle, /**< The threshold angle which is used to detect the robot lying on the ground */
  }),
});

/**
 * @class FallDownStateDetector
 *
 * A module for computing the current body state from sensor data
 */
class FallDownStateDetector: public FallDownStateDetectorBase
{
private:
  ENUM(KeeperJumped,
  {,
    None,
    KeeperJumpedLeft,
    KeeperJumpedRight,
  });

  unsigned lastFallDetected = 0;
  KeeperJumped keeperJumped = None; /**< Whether the keeper has recently executed a jump motion that has to be integrated in odometry offset. */
  RingBufferWithSum<float, 15> accXbuffer, accYbuffer, accZbuffer; /**< Buffers for averaging sensor data */

  void update(FallDownState& fallDownState);

  bool isFalling();
  bool isStaggering();
  bool specialSpecialAction();
  bool isUprightOrStaggering(FallDownState& fallDownState);
  FallDownState::Direction directionOf(Vector2a angle);
  FallDownState::Sidestate sidewardsOf(FallDownState::Direction dir);
};
