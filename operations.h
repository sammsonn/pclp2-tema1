#ifndef _OPERATIONS_H_
#define _OPERATIONS_H_

void tire_pressure_status(void *data);
void tire_temperature_status(void *data);
void tire_wear_level_status(void *data);
void tire_performance_score(void *data);
void pmu_compute_power(void *data);
void pmu_regenerate_energy(void *data);
void pmu_get_energy_usage(void *data);
void pmu_is_battery_healthy(void *data);
void get_operations(void **operations);

#endif // _OPERATIONS_H_
