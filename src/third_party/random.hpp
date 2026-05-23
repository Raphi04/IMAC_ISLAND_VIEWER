#pragma once
#include <glm/glm.hpp>
#include <random>


namespace p6::random {

/* ------------------------------- */
/** \defgroup random Random
 * Pick random numbers, points, directions, etc.
 * @{*/
/* ------------------------------- */

/// Returns a random number between 0 and 1.
float number();

/// Returns a random number between 0 and `max`.
/// Throws a `std::invalid_argument` exception if `max < 0`.
float number(float max);

/// Returns a random number between `min` and `max`.
/// Throws a `std::invalid_argument` exception if `min > max`.
float number(float min, float max);

/// Returns a random integer between 0 (included) and `max` (excluded).
/// Throws a `std::invalid_argument` exception if `max <= 0`.
int integer(int max);

/// Returns a random integer between `min` (included) and `max` (excluded).
/// Throws a `std::invalid_argument` exception if `min >= max`.
int integer(int min, int max);

/// Returns a random size_t between 0 (included) and `max` (excluded).
size_t size_type(size_t max);

/// Returns a random size_t between `min` (included) and `max` (excluded).
/// Throws a `std::invalid_argument` exception if `min >= max`.
size_t size_type(size_t min, size_t max);

/// Randomly returns one of the elements of the `collection`.
/// Throws a `std::invalid_argument` exception if the `collection` is empty.

/**@}*/
} // namespace p6::random