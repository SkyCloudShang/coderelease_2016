/*
 * FootContactModelProvider.h
 *
 *  Created on: Mar 14, 2012
 *      Author: arne
 *              simont@tzi.de
 */

#pragma once

#include "Representations/Configuration/DamageConfiguration.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Infrastructure/GameInfo.h"
#include "Representations/Infrastructure/SensorData/KeyStates.h"
#include "Representations/MotionControl/MotionInfo.h"
#include "Representations/Sensing/FallDownState.h"
#include "Representations/Sensing/FootContactModel.h"
#include "Tools/RingBufferWithSum.h"
#include "Tools/Module/Module.h"

/** Number of contacts to buffer. 100 complies to 1 second */
#define BUFFER_SIZE 100

MODULE(FootContactModelProvider,
{,
  REQUIRES(MotionInfo),
  REQUIRES(FallDownState),
  REQUIRES(FrameInfo),
  REQUIRES(KeyStates),
  REQUIRES(GameInfo),
  REQUIRES(DamageConfigurationBody),
  PROVIDES(FootContactModel),
  DEFINES_PARAMETERS(
  {,
    (bool)(false) debug,             /**< enables debug mode (debug sound) */
    (int)(15) contactThreshold,      /**< threshold in contacts per second to determine foot contact */
    (int)(250) malfunctionThreshold, /**< threshold in Motion frames of contact after a malfunction of a bumper is detected (2.5 seconds) */
    (unsigned)(1000) soundDelay,     /**< Delay between debug sounds. */
  }),
});

/**
 * Provides information about foot contacts :)
 */
class FootContactModelProvider : public FootContactModelProviderBase
{
public:
  FootContactModelProvider();

private:
  void update(FootContactModel& model);

  /**
   * Buffers the contacts over the last frames.
   * Each frame one value is added to this buffer.
   * A 1 if there was a foot contact in the frame, 0 otherwise.
   */
  RingBufferWithSum<int, BUFFER_SIZE> contactBufferLeft;
  RingBufferWithSum<int, BUFFER_SIZE> contactBufferRight;

  int contactDurationLeft;                             /**< the duration of the last contact. Will be reset to 0 if contact is lost */
  int contactDurationRight;                            /**< the duration of the last contact. Will be reset to 0 if contact is lost */

  int leftFootLeftDuration;                            /**< duration of consecutive contact detections for each bumper */
  int leftFootRightDuration;
  int rightFootLeftDuration;
  int rightFootRightDuration;

  unsigned int lastSoundTime;                          /**< Delay between debug sounds. */

  /**
  * Checks for contact of a bumper. Additionally, if a bumper is pressed longer than the
  * malfunction threshold, it is ignored (i.e. this method returns false).
  *
  * @param key The key to check.
  * @param duration The duration how long this key has been pressed. This value will be incremented by
  *     this method (if bumper has contact) or reset to 0 (if bumper has no contact)
  * @returns true if the key is pressed and its pressed duration is not longer than the malfunctionThreshold.
  */
  bool checkContact(KeyStates::Key key, int& duration);
};
