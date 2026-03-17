/*
 * madgwick.h
 *
 *  Created on: 2026年3月10日
 *      Author: 18701
 */

#ifndef INC_MADGWICK_H_
#define INC_MADGWICK_H_

#include <stdint.h>
#include <math.h>

// Beta is the algorithm gain.
// - Higher = trusts accelerometer more (faster response to gravity, but more noise from motor vibration)
// - Lower = trusts gyroscope more (smoother, but drifts slightly more)
// 0.1f is the standard starting point.
#define MADGWICK_BETA 0.5f

// A struct to hold your final human-readable angles
typedef struct {
    float roll;
    float pitch;
    float yaw;
} EulerAngles_t;

// Function Prototypes
void Madgwick_Init(void);
void Madgwick_Update(float gx, float gy, float gz, float ax, float ay, float az, float dt);
void Madgwick_ComputeAngles(EulerAngles_t *angles);

#endif /* INC_MADGWICK_H_ */
