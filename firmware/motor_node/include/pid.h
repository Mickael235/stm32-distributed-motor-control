#ifndef PID_H
#define PID_H

typedef struct
{
    /* Gains du correcteur */
    float kp;
    float ki;
    float kd;

    /* Période d'échantillonnage en secondes */
    float ts;

    /* État interne */
    float integrator;
    float previous_error;

    /* Limites de sortie */
    float output_min;
    float output_max;

} PID_Controller_t;


/**
 * @brief Initialise un contrôleur PID.
 *
 * @param pid         Pointeur vers la structure PID.
 * @param kp          Gain proportionnel.
 * @param ki          Gain intégral.
 * @param kd          Gain dérivé.
 * @param ts          Période d'échantillonnage en secondes.
 * @param output_min  Limite minimale de sortie.
 * @param output_max  Limite maximale de sortie.
 */
void PID_Init(
    PID_Controller_t *pid,
    float kp,
    float ki,
    float kd,
    float ts,
    float output_min,
    float output_max
);


/**
 * @brief Calcule une nouvelle commande PID.
 *
 * @param pid         Pointeur vers le contrôleur PID.
 * @param setpoint    Consigne.
 * @param measurement Mesure actuelle.
 *
 * @return Commande PID saturée.
 */
float PID_Update(
    PID_Controller_t *pid,
    float setpoint,
    float measurement
);


/**
 * @brief Réinitialise les états internes du PID.
 *
 * @param pid Pointeur vers le contrôleur PID.
 */
void PID_Reset(PID_Controller_t *pid);

#endif /* PID_H */
