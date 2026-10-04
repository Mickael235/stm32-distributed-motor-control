#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "pid.h"


#define FLOAT_TOLERANCE 0.0001f


static void assert_float_close(float actual, float expected)
{
    assert(fabsf(actual - expected) < FLOAT_TOLERANCE);
}


/*
 * Test 1
 * Erreur nulle :
 *
 * setpoint = measurement
 * => sortie attendue = 0
 */
static void test_zero_error(void)
{
    PID_Controller_t pid;

    PID_Init(
        &pid,
        1.0f,
        0.0f,
        0.0f,
        0.01f,
        -100.0f,
        100.0f
    );

    float output = PID_Update(
        &pid,
        100.0f,
        100.0f
    );

    assert_float_close(output, 0.0f);

    printf("[PASS] zero error\n");
}


/*
 * Test 2
 * Erreur positive :
 *
 * erreur = 100 - 80 = 20
 * Kp = 1
 * => sortie = +20
 */
static void test_positive_error(void)
{
    PID_Controller_t pid;

    PID_Init(
        &pid,
        1.0f,
        0.0f,
        0.0f,
        0.01f,
        -100.0f,
        100.0f
    );

    float output = PID_Update(
        &pid,
        100.0f,
        80.0f
    );

    assert_float_close(output, 20.0f);

    printf("[PASS] positive error\n");
}


/*
 * Test 3
 * Erreur négative :
 *
 * erreur = 80 - 100 = -20
 * => sortie = -20
 */
static void test_negative_error(void)
{
    PID_Controller_t pid;

    PID_Init(
        &pid,
        1.0f,
        0.0f,
        0.0f,
        0.01f,
        -100.0f,
        100.0f
    );

    float output = PID_Update(
        &pid,
        80.0f,
        100.0f
    );

    assert_float_close(output, -20.0f);

    printf("[PASS] negative error\n");
}


/*
 * Test 4
 * Saturation positive.
 */
static void test_positive_saturation(void)
{
    PID_Controller_t pid;

    PID_Init(
        &pid,
        10.0f,
        0.0f,
        0.0f,
        0.01f,
        -100.0f,
        100.0f
    );

    float output = PID_Update(
        &pid,
        100.0f,
        0.0f
    );

    assert_float_close(output, 100.0f);

    printf("[PASS] positive saturation\n");
}


/*
 * Test 5
 * Saturation négative.
 */
static void test_negative_saturation(void)
{
    PID_Controller_t pid;

    PID_Init(
        &pid,
        10.0f,
        0.0f,
        0.0f,
        0.01f,
        -100.0f,
        100.0f
    );

    float output = PID_Update(
        &pid,
        -100.0f,
        0.0f
    );

    assert_float_close(output, -100.0f);

    printf("[PASS] negative saturation\n");
}


/*
 * Test 6
 * Accumulation de l'intégrateur.
 *
 * Ki = 1
 * Ts = 1 s
 * erreur = 10
 *
 * premier appel : I = 10
 * second appel  : I = 20
 */
static void test_integrator_accumulation(void)
{
    PID_Controller_t pid;

    PID_Init(
        &pid,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        -1000.0f,
        1000.0f
    );

    float output1 = PID_Update(
        &pid,
        10.0f,
        0.0f
    );

    float output2 = PID_Update(
        &pid,
        10.0f,
        0.0f
    );

    assert_float_close(output1, 10.0f);
    assert_float_close(output2, 20.0f);
    assert_float_close(pid.integrator, 20.0f);

    printf("[PASS] integrator accumulation\n");
}


/*
 * Test 7
 * Anti-windup.
 *
 * Une erreur importante provoque une saturation.
 * L'intégrateur ne doit pas continuer à augmenter
 * dans le sens de cette saturation.
 */
static void test_anti_windup(void)
{
    PID_Controller_t pid;

    PID_Init(
        &pid,
        10.0f,
        5.0f,
        0.0f,
        1.0f,
        -100.0f,
        100.0f
    );

    float output1 = PID_Update(
        &pid,
        100.0f,
        0.0f
    );

    float integrator_after_first_update = pid.integrator;

    float output2 = PID_Update(
        &pid,
        100.0f,
        0.0f
    );

    assert_float_close(output1, 100.0f);
    assert_float_close(output2, 100.0f);

    /*
     * L'intégrateur ne doit pas continuer
     * à augmenter pendant la saturation.
     */
    assert_float_close(
        pid.integrator,
        integrator_after_first_update
    );

    printf("[PASS] anti-windup\n");
}


/*
 * Test 8
 * Reset du PID.
 */
static void test_reset(void)
{
    PID_Controller_t pid;

    PID_Init(
        &pid,
        1.0f,
        1.0f,
        0.0f,
        0.1f,
        -100.0f,
        100.0f
    );

    PID_Update(
        &pid,
        100.0f,
        50.0f
    );

    PID_Reset(&pid);

    assert_float_close(pid.integrator, 0.0f);
    assert_float_close(pid.previous_error, 0.0f);

    printf("[PASS] PID reset\n");
}


/*
 * Test 9
 * Retour à erreur nulle.
 */
static void test_return_to_zero_error(void)
{
    PID_Controller_t pid;

    PID_Init(
        &pid,
        1.0f,
        0.0f,
        0.0f,
        0.01f,
        -100.0f,
        100.0f
    );

    float output1 = PID_Update(
        &pid,
        100.0f,
        80.0f
    );

    float output2 = PID_Update(
        &pid,
        100.0f,
        100.0f
    );

    assert(output1 > 0.0f);
    assert_float_close(output2, 0.0f);

    printf("[PASS] return to zero error\n");
}


/*
 * Tests supplémentaires de robustesse.
 */
static void test_null_pointer(void)
{
    float output = PID_Update(
        NULL,
        100.0f,
        0.0f
    );

    assert_float_close(output, 0.0f);

    printf("[PASS] null pointer protection\n");
}


static void test_invalid_sample_time(void)
{
    PID_Controller_t pid;

    PID_Init(
        &pid,
        1.0f,
        1.0f,
        1.0f,
        0.0f,
        -100.0f,
        100.0f
    );

    float output = PID_Update(
        &pid,
        100.0f,
        0.0f
    );

    assert_float_close(output, 0.0f);

    printf("[PASS] invalid sample time protection\n");
}


int main(void)
{
    printf("=== PID UNIT TESTS ===\n\n");

    test_zero_error();
    test_positive_error();
    test_negative_error();

    test_positive_saturation();
    test_negative_saturation();

    test_integrator_accumulation();
    test_anti_windup();

    test_reset();
    test_return_to_zero_error();

    test_null_pointer();
    test_invalid_sample_time();

    printf("\n=== ALL PID TESTS PASSED ===\n");

    return 0;
}
