#include <Windows.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <tchar.h>
#include <string>
#include <iomanip>

#define DISK_NAME_SIZE 4 // Кількість символів, що займе назва диску
// 1 - С 2 - : 3 - \ 4 - 0

// Інформація про диск
struct DiskInf {
	TCHAR name[DISK_NAME_SIZE]; // Його назва
	std::string type; // Тип
	TCHAR fileSystemName[15]; // Тип файлової системи
	DWORD serialNumber; // Серійний номер
	DWORD systemFlags; // Системні прапорці
	DWORD bytesPerSector; // К-сть байт в секторів
	DWORD sectorsPerCluster; // Секторів у кластері
	DWORD freeClusters; // Вільні кластери
	DWORD totalClusters; // Загальна кількість кластерів
	__int64 totalSpace; // Загальний розмір диску
	__int64 freeSpace; // Вільний простір на ньому
	__int64 usedSpace; // Зайнятий
};

// Виводить інформацію про диски
void printInf(std::vector<DiskInf>& disks) {
	for (DiskInf& disk : disks) {
		std::cout << std::setw(30) << std::left << "Disk name: " << std::right << disk.name
			<< "\n" << std::setw(30) << std::left << "Type: " << std::right << disk.type
			<< "\n" << std::setw(30) << std::left << "File system: " << std::right << disk.fileSystemName
			<< "\n" << std::setw(30) << std::left << "Serial number: " << std::right << disk.serialNumber
			<< "\n" << std::setw(30) << std::left << "System flags: " << std::right << disk.systemFlags
			<< "\n" << std::setw(30) << std::left << "Bytes per cluster: " << std::right << disk.bytesPerSector
			<< "\n" << std::setw(30) << std::left << "Sectors per cluster: " << std::right << disk.sectorsPerCluster
			<< "\n" << std::setw(30) << std::left << "Free clusters: " << std::right << disk.freeClusters
			<< "\n" << std::setw(30) << std::left << "Total number of clusters: " << std::right << disk.totalClusters
			<< "\n" << std::setw(30) << std::left << "Total space: " << std::right << disk.totalSpace / (1024 * 1024) << " MB" << " (" << static_cast<long double>(disk.totalSpace) / pow(1024, 3) << " GB)"
			<< "\n" << std::setw(30) << std::left << "Free space: " << std::right << disk.freeSpace / (1024 * 1024) << " MB" << " (" << static_cast<long double>(disk.freeSpace) / pow(1024, 3) << " GB)"
			<< "\n" << std::setw(30) << std::left << "Used space: " << std::right << disk.usedSpace / (1024 * 1024) << " MB" << " (" << static_cast<long double>(disk.usedSpace) / pow(1024, 3) << " GB)\n"
			<< std::endl;
	}
}

// Визначає тип диску
std::string driveType(int type) {
	switch (type) {
		case 0:
			return "DRIVE_UNKNOWN";
		case 1:
			return "DRIVE_NO_ROOT_DIR";
		case 2:
			return "DRIVE_REMOVABLE";
		case 3:
			return "DRIVE_FIXED";
		case 4:
			return "DRIVE_REMOTE";
		case 5:
			return "DRIVE_CDROM";
		case 6:
			return "DRIVE_RAMDISK";
	}
}

// Отримуємо інформацію про диски
void getDisksInf(std::vector<DiskInf>& disks) {
	LPSTR diskNames = new TCHAR;
	DWORD disksNum = GetLogicalDriveStrings(MAX_PATH, diskNames) / DISK_NAME_SIZE; // Рахуємо кількість дисків та зчитуємо їхні назви

	for (int i = 0; i < disksNum; i++) {
		DiskInf disk{}; // Інформація про диск

		_tcscpy_s(disk.name, diskNames); // Копіюємо ім'я
		disk.type = driveType(GetDriveType(disk.name)); // отримуємо тип диску

		GetVolumeInformation(disk.name, NULL, NULL, &disk.serialNumber, NULL,
			&disk.systemFlags, disk.fileSystemName, sizeof(disk.fileSystemName)); // Отримуємо іншу інформацію для цього диску

		GetDiskFreeSpaceEx(disk.name, (PULARGE_INTEGER)&disk.freeSpace, (PULARGE_INTEGER)&disk.totalSpace, NULL); // Отримуємо загальний простір диску та вільний
		disk.usedSpace = disk.totalSpace - disk.freeSpace; // Рахуємо, скільки зайнято місця на диску

		GetDiskFreeSpace(disk.name, &disk.sectorsPerCluster, &disk.bytesPerSector, &disk.freeClusters, &disk.totalClusters); // Отримуємо інформацію про кластери

		disks.push_back(disk); // Додаємо цей диск у масив дисків

		diskNames += sizeof(TCHAR) * DISK_NAME_SIZE; // Записуємо назву наступного диску, що знаходиться в рядку
	}
}

int main() {
	std::vector<DiskInf> disks; // Масив дисків

	getDisksInf(disks); // Отримуємо інформацію про диски
	printInf(disks); // Виводимо інформацію про диски в консоль

	return 0;
}