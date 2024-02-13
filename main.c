#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "operations.h"

void print(sensor *sensors, int index)
{
	// Verifică dacă tipul de senzor este TIRE
	if (sensors[index].sensor_type == TIRE)
	{
		// Afișează detaliile senzorului TIRE
		printf("Tire Sensor\n");
		printf("Pressure: %.2f\n",
		((tire_sensor *)sensors[index].sensor_data)->pressure);
		printf("Temperature: %.2f\n",
		((tire_sensor *)sensors[index].sensor_data)->temperature);
		printf("Wear Level: %d%%\n",
		((tire_sensor *)sensors[index].sensor_data)->wear_level);
		printf("Performance Score: ");
		// Afișează scorul de performanță dacă a fost calculat,
		// în caz contrar se afișează „Not Calculated”
		if (((tire_sensor *)sensors[index].sensor_data)->performace_score != 0)
		{
			printf("%d\n",
			((tire_sensor *)sensors[index].sensor_data)->performace_score);
		}
		else
		{
			printf("Not Calculated\n");
		}
	}
	// Verifică dacă tipul de senzor este PMU
	else if (sensors[index].sensor_type == PMU)
	{
		// Afișează detaliile senzorului PMU
		printf("Power Management Unit\n");
		printf("Voltage: %.2f\n",
		((power_management_unit *)sensors[index].sensor_data)->voltage);
		printf("Current: %.2f\n",
		((power_management_unit *)sensors[index].sensor_data)->current);
		printf("Power Consumption: %.2f\n", ((power_management_unit *)
		sensors[index].sensor_data)->power_consumption);
		printf("Energy Regen: %d%%\n",
		((power_management_unit *)sensors[index].sensor_data)->energy_regen);
		printf("Energy Storage: %d%%\n",
		((power_management_unit *)sensors[index].sensor_data)->energy_storage);
	}
}

void analyze(sensor *sensors, int index, void (*operations[])(void *))
{
	int j;
	// Parcurge toate operațiunile pentru senzor la indexul dat
	for (j = 0; j < sensors[index].nr_operations; j++)
	{
		// Apelează operația corespunzătoare pentru indexul dat
		operations[sensors[index].operations_idxs[j]]
		(sensors[index].sensor_data);
	}
}

void clear(sensor **sensors, int *sensors_number)
{
	int i, error, j;
	sensor *aux;
	// Parcurge toți senzorii
	for (i = 0; i < *sensors_number; i++)
	{
		error = 0;
		// Verifică dacă tipul de senzor este TIRE
		if ((*sensors)[i].sensor_type == TIRE)
		{
			// Verifică dacă datele senzorului TIRE se încadrează
			// în intervale valide, setează indicatorul de eroare dacă nu
			if (((tire_sensor *)(*sensors)[i].sensor_data)->pressure < 19
			|| ((tire_sensor *)(*sensors)[i].sensor_data)->pressure > 28)
			{
				error = 1;
			}
			else if(((tire_sensor *)(*sensors)[i].sensor_data)->temperature < 0
			|| ((tire_sensor *)(*sensors)[i].sensor_data)->temperature > 120)
			{
				error = 1;
			}
			else if(((tire_sensor *)(*sensors)[i].sensor_data)->wear_level < 0
			|| ((tire_sensor *)(*sensors)[i].sensor_data)->wear_level > 100)
			{
				error = 1;
			}
		}
		// Verifică dacă tipul de senzor este PMU
		else if ((*sensors)[i].sensor_type == PMU)
		{
			// Verifică dacă datele senzorului PMU se află
			// în intervale valide, setează indicatorul de eroare dacă nu
			if (((power_management_unit *)(*sensors)[i].
			sensor_data)->voltage < 10 || ((power_management_unit *)
			(*sensors)[i].sensor_data)->voltage > 20)
			{
				error = 1;
			}
			else if (((power_management_unit *)(*sensors)[i].
			sensor_data)->current < -100 || ((power_management_unit *)
			(*sensors)[i].sensor_data)->current > 100)
			{
				error = 1;
			}
			else if (((power_management_unit *)(*sensors)[i].
			sensor_data)->power_consumption < 0 || ((power_management_unit *)
			(*sensors)[i].sensor_data)->power_consumption > 1000)
			{
				error = 1;
			}
			else if (((power_management_unit *)(*sensors)[i]
			.sensor_data)->energy_regen < 0 || ((power_management_unit *)
			(*sensors)[i].sensor_data)->energy_regen > 100)
			{
				error = 1;
			}
			else if (((power_management_unit *)(*sensors)[i].
			sensor_data)->energy_storage < 0 || ((power_management_unit *)
			(*sensors)[i].sensor_data)->energy_storage > 100)
			{
				error = 1;
			}
		}
		// Dacă a fost detectată o eroare,
		// eliberează memoria senzorului defect și o scoate din vector
		if (error)
		{
			aux = &((*sensors)[i]);
			free(aux->sensor_data);
			free(aux->operations_idxs);

			for (j = i; j < *sensors_number - 1; j++)
			{
				(*sensors)[j] = (*sensors)[j + 1];
			}

			*sensors_number = *sensors_number - 1;
			*sensors = (sensor *)
			realloc(*sensors, *sensors_number * sizeof(sensor));
			i--;
		}
	}
}

// Această funcție eliberează memoria alocată pentru datele senzorului
// și indicii de operații pentru fiecare senzor
void free_sensors(sensor *sensors, int sensors_number)
{
	int i;
	// Parcurge toți senzorii
	for (i = 0; i < sensors_number; i++)
	{
		// Eliberează memoria alocată pentru
		// datele senzorului și indicii de operații
		free(sensors[i].sensor_data);
		free(sensors[i].operations_idxs);
	}
	// Eliberează memoria alocată pentru vectorul de senzori
	free(sensors);
}

// Funcția principală execută codul, citește intrările de la tastatură și din
// fișierul binar și apelează alte funcții pentru a manipula și analiza datele
// senzorului. În cele din urmă, eliberează memoria alocată și returnează 0.
int main(int argc, char const *argv[])
{
	int sensors_number, i, j, k, index, tires_number = 0, pmus_number = 0;
	char command[15];
	void (*operations[8])(void *);
	sensor *sensors = NULL, *tire_sensors = NULL, *pmu_sensors = NULL;
	FILE *fb;

	// Deschide un fișier binar pentru citire
	fb = fopen(argv[1], "rb");

	// Citește numărul de senzori din fișierul binar
	fread(&sensors_number, sizeof(int), 1, fb);

	sensors = (sensor *)malloc(sensors_number * sizeof(sensor));
	tire_sensors = (sensor *)malloc(sensors_number * sizeof(sensor));
	pmu_sensors = (sensor *)malloc(sensors_number * sizeof(sensor));

	get_operations((void*)operations);

	for (i = 0; i < sensors_number; i++)
	{
		fread(&sensors[i].sensor_type, sizeof(enum sensor_type), 1, fb);

		if (sensors[i].sensor_type == TIRE) // Dacă senzorul este unul TIRE
		{
			tires_number++;
			tire_sensor *t = (tire_sensor *)malloc(sizeof(tire_sensor));
			tire_sensors[tires_number - 1].sensor_data = t;
			tire_sensors[tires_number - 1].sensor_type = TIRE;

			// Citește datele senzorului TIRE din fișierul binar
			fread(&((tire_sensor *)tire_sensors[tires_number - 1].
			sensor_data)->pressure, sizeof(float), 1, fb);
			fread(&((tire_sensor *)tire_sensors[tires_number - 1].
			sensor_data)->temperature, sizeof(float), 1, fb);
			fread(&((tire_sensor *)tire_sensors[tires_number - 1].
			sensor_data)->wear_level, sizeof(int), 1, fb);
			fread(&((tire_sensor *)tire_sensors[tires_number - 1].
			sensor_data)->performace_score, sizeof(int), 1, fb);

			fread(&tire_sensors[tires_number - 1].nr_operations,
			sizeof(int), 1, fb);
			tire_sensors[tires_number - 1].operations_idxs = (int *)
			malloc(tire_sensors[tires_number - 1].nr_operations * sizeof(int));

			for (j = 0; j < tire_sensors[tires_number - 1].nr_operations; j++)
			{
				fread(&tire_sensors[tires_number - 1].operations_idxs[j], 
				sizeof(int), 1, fb);
			}
		}
		// Dacă senzorul este un de tip PMU
		else if (sensors[i].sensor_type == PMU) 
		{
			pmus_number++;
			power_management_unit *t = (power_management_unit *)
			malloc(sizeof(power_management_unit));
			pmu_sensors[pmus_number - 1].sensor_data = t;
			pmu_sensors[pmus_number - 1].sensor_type = PMU;

			// Citește datele senzorului PMU din fișierul binar
			fread(&((power_management_unit *)pmu_sensors[pmus_number - 1]
			.sensor_data)->voltage, sizeof(float), 1, fb);
			fread(&((power_management_unit *)pmu_sensors[pmus_number - 1]
			.sensor_data)->current, sizeof(float), 1, fb);
			fread(&((power_management_unit *)pmu_sensors[pmus_number - 1]
			.sensor_data)->power_consumption, sizeof(float), 1, fb);
			fread(&((power_management_unit *)pmu_sensors[pmus_number - 1]
			.sensor_data)->energy_regen, sizeof(int), 1, fb);
			fread(&((power_management_unit *)pmu_sensors[pmus_number - 1]
			.sensor_data)->energy_storage, sizeof(int), 1, fb);

			fread(&pmu_sensors[pmus_number - 1].nr_operations,
			sizeof(int), 1, fb);
			pmu_sensors[pmus_number - 1].operations_idxs = (int *)
			malloc(pmu_sensors[pmus_number - 1].nr_operations * sizeof(int));

			for (j = 0; j < pmu_sensors[pmus_number - 1].nr_operations; j++)
			{
				fread(&pmu_sensors[pmus_number - 1].operations_idxs[j],
				sizeof(int), 1, fb);
			}
		}
	}

	// Copiază valorile din pmu_sensors în vectorul de senzori
	for (i = 0; i < pmus_number; i++)
	{
		sensors[i] = pmu_sensors[i];
	}

	// Copiază valorile din tire_sensors în vectorul de senzori
	for (k = 0; k < tires_number; k++)
	{
		sensors[i] = tire_sensors[k];
		i++;
	}

	// Citește comenzile până la întălnirea "exit"
	fgets(command, 15, stdin);
	command[strlen(command) - 1] = '\0';
	while (strcmp(command, "exit"))
	{
		if (strstr(command, "print"))
		{
			sscanf(command, "print %d", &index);
			if (index < 0 || index > sensors_number)
			{
				printf("Index not in range!\n");
			}
			else
			{
				print(sensors, index);
			}
		}
		else if (strstr(command, "analyze"))
		{
			sscanf(command, "analyze %d", &index);
			analyze(sensors, index, operations);
		}
		else if (!strcmp(command, "clear"))
		{
			clear(&sensors, &sensors_number);
		}

		fgets(command, 15, stdin);
		command[strlen(command) - 1] = '\0';
	}

	// Închide fișierul binar
	fclose(fb);

	// Eliberează memoria alocată
	free_sensors(sensors, sensors_number);
	free(tire_sensors);
	free(pmu_sensors);

	return 0;
}