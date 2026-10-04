#include "pid.h"


static float PID_Clamp(float value, float min_value, float max_value)
{
    if (value > max_value)
    {
        return max_value;
    }

    if (value < min_value)
    {
        return min_value;
    }

    return value;
}


void PID_Init(
    PID_Controller_t *pid,
    float kp,
    float ki,
    float kd,
    float ts,
    float output_min,
    float output_max
)
{
    if (pid == 0)
    {
        return;
    }

    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;

    pid->ts = ts;

    pid->output_min = output_min;
    pid->output_max = output_max;

    pid->integrator = 0.0f;
    pid->previous_error = 0.0f;
}


float PID_Update(
    PID_Controller_t *pid,
    float setpoint,
    float measurement
)
{
    float error;
    float proportional;
    float derivative;
    float integral_increment;
    float candidate_integrator;
    float unsaturated_output;
    float output;

    if (pid == 0)
    {
        return 0.0f;
    }

    if (pid->ts <= 0.0f)
    {
        return 0.0f;
    }

    /* Erreur */
    error = setpoint - measurement;

    /* Terme proportionnel */
    proportional = pid->kp * error;

    /* Terme dérivé */
    derivative =
        pid->kd *
        (error - pid->previous_error) /
        pid->ts;

    /* Candidat pour l'intégrateur */
    integral_increment =
        pid->ki *
        pid->ts *
        error;

    candidate_integrator =
        pid->integrator +
        integral_increment;

    /* Sortie avant saturation */
    unsaturated_output =
        proportional +
        candidate_integrator +
        derivative;

    /*
     * Anti-windup par intégration conditionnelle.
     *
     * On ne mémorise pas la nouvelle valeur de l'intégrateur
     * si elle pousse davantage la sortie dans une saturation.
     */
    if (!((unsaturated_output > pid->output_max &&
           integral_increment > 0.0f) ||
          (unsaturated_output < pid->output_min &&
           integral_increment < 0.0f)))
    {
        pid->integrator = candidate_integrator;
    }

    /*
     * La commande instantanée reste calculée à partir de
     * la sortie non saturée, puis limitée.
     */
    output = PID_Clamp(
        unsaturated_output,
        pid->output_min,
        pid->output_max
    );

    pid->previous_error = error;

    return output;
}



void PID_Reset(PID_Controller_t *pid)
{
    if (pid == 0)
    {
        return;
    }

    pid->integrator = 0.0f;
    pid->previous_error = 0.0f;
}
