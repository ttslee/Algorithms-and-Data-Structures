#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <future>
#include <iostream>
#include <vector>

static std::string words[5] = { "clock", "door", "soccer ball", "car", "boat" };

static double price[5] = { 50.0, 190.0, 25.99, 45000.00, 110000.00 };

static std::mutex mu;

static std::condition_variable cv;