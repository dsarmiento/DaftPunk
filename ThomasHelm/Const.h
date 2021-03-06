#include "Arduino.h"

uint8_t colon[8] = {B00000000, B00011000, B00011000, B00000000, B00000000, B00011000, B00011000, B00000000};  // :
uint8_t numbers[10][8] = {{B01111110, B11111111, B11100111, B11100111, B11100111, B11100111, B11111111, B01111110},  // 0
                          {B00001110, B00000111, B00000111, B00000111, B00000111, B00000111, B00000111, B00000111},  // 1
                          {B11111110, B11111111, B00000111, B01111111, B11111110, B11100000, B11111111, B11111111},  // 2
                          {B11111110, B11111111, B00000111, B00111110, B00111110, B00000111, B11111111, B11111110},  // 3
                          {B11100111, B11100111, B11100111, B11111111, B01111111, B00000111, B00000111, B00000111},  // 4
                          {B11111111, B11111111, B11110000, B11111110, B01111111, B00000111, B11111111, B11111110},  // 5
                          {B00111110, B01111110, B11100000, B11111110, B11111111, B11100111, B11100111, B01111110},  // 6
                          {B01111111, B01111111, B00000111, B00001111, B00011110, B00111100, B01111000, B01110000},  // 7
                          {B01111110, B11100111, B11100111, B01111110, B01111110, B11100111, B11100111, B01111110},  // 8
                          {B01111110, B11100111, B11100111, B11111111, B01111111, B00000111, B01111110, B01111100}}; // 9
uint8_t alphabet[26][8] = {{B00001110, B00010001, B00010001, B00011111, B00010001, B00010001, B00010001, B00010001},  // A
                           {B00011110, B00010001, B00010001, B00011110, B00010001, B00010001, B00010001, B00011110},  // B
                           {B00001111, B00010000, B00010000, B00010000, B00010000, B00010000, B00010000, B00001111},  // C
                           {B00011110, B00010001, B00010001, B00010001, B00010001, B00010001, B00010001, B00011110},  // D
                           {B00011111, B00010000, B00010000, B00011110, B00010000, B00010000, B00010000, B00011111},  // E
                           {B00011111, B00010000, B00010000, B00011111, B00010000, B00010000, B00010000, B00010000},  // F
                           {B00001110, B00010001, B00010000, B00010000, B00010111, B00010001, B00010001, B00001110},  // G
                           {B00010001, B00010001, B00010001, B00011111, B00010001, B00010001, B00010001, B00010001},  // H
                           {B00011111, B00000100, B00000100, B00000100, B00000100, B00000100, B00000100, B00011111},  // I
                           {B00011111, B00000010, B00000010, B00000010, B00000010, B00000010, B00010010, B00011110},  // J
                           {B00010001, B00010010, B00010100, B00011000, B00011000, B00010100, B00010010, B00010001},  // K
                           {B00010000, B00010000, B00010000, B00010000, B00010000, B00010000, B00010000, B00011111},  // L
                           {B00010001, B00011011, B00010101, B00010001, B00010001, B00010001, B00010001, B00010001},  // M
                           {B00010001, B00010001, B00011001, B00010101, B00010011, B00010001, B00010001, B00010001},  // N
                           {B00001110, B00010001, B00010001, B00010001, B00010001, B00010001, B00010001, B00001110},  // O
                           {B00011110, B00010001, B00010001, B00011110, B00010000, B00010000, B00010000, B00010000},  // P
                           {B00001110, B00010001, B00010001, B00010001, B00010001, B00010101, B00010010, B00001101},  // Q
                           {B00011110, B00010001, B00010001, B00011110, B00011000, B00010100, B00010010, B00010001},  // R
                           {B00001111, B00010000, B00010000, B00001110, B00000001, B00000001, B00000001, B00011110},  // S
                           {B00011111, B00000100, B00000100, B00000100, B00000100, B00000100, B00000100, B00000100},  // T
                           {B00010001, B00010001, B00010001, B00010001, B00010001, B00010001, B00010001, B00001110},  // U
                           {B00010001, B00010001, B00010001, B00010001, B00001010, B00001010, B00001010, B00000100},  // V
                           {B00010001, B00010001, B00010001, B00010001, B00010101, B00010101, B00010101, B00001010},  // W
                           {B00010001, B00001010, B00001010, B00000100, B00001010, B00001010, B00001010, B00010001},  // X
                           {B00010001, B00010001, B00001010, B00001010, B00000100, B00000100, B00000100, B00000100},  // Y
                           {B00011111, B00000001, B00000010, B00000100, B00001000, B00010000, B00010000, B00011111}}; // Z
uint8_t heartbeat[8][5] = {{B00000000, B00000010, B00000000, B00000000, B01000000},
                           {B00000000, B00000101, B00000000, B00000000, B10100000},
                           {B00000000, B00001000, B10000000, B00000001, B00010000},
                           {B11110000, B00010000, B01111110, B00000010, B00001111},
                           {B00001000, B00100000, B00000001, B00000100, B00000000},
                           {B00000100, B01000000, B00000000, B10001000, B00000000},
                           {B00000010, B10000000, B00000000, B01010000, B00000000},
                           {B00000001, B00000000, B00000000, B00100000, B00000000}};
uint8_t heartEmpty[8][2] = {{B01110011, B10000000},
                            {B10001100, B01000000},
                            {B10000000, B01000000},
                            {B10000000, B01000000},
                            {B01000000, B10000000},
                            {B00100001, B00000000},
                            {B00010010, B00000000},
                            {B00001100, B00000000}};
uint8_t heartFull[8][2] = {{B01110011, B10000000},
                           {B11111111, B11000000},
                           {B11111111, B11000000},
                           {B11111111, B11000000},
                           {B01111111, B10000000},
                           {B00111111, B00000000},
                           {B00011110, B00000000},
                           {B00001100, B00000000}};
uint8_t blinking[4][8] = {{B00111100,
                           B01000010,
                           B01000010,
                           B01011010,
                           B01011010,
                           B01000010,
                           B01000010,
                           B00111100},
                          {B00000000,
                           B00111100,
                           B01000010,
                           B01011010,
                           B01011010,
                           B01000010,
                           B00111100,
                           B00000000},
                          {B00000000,
                           B00000000,
                           B00111100,
                           B01000010,
                           B01000010,
                           B00111100,
                           B00000000,
                           B00000000},
                          {B00000000,
                           B00000000,
                           B00000000,
                           B01111110,
                           B01111110,
                           B00000000,
                           B00000000,
                           B00000000}};
uint8_t pacMan[4][8] = {{B00111100,
                         B01000100,
                         B10001000,
                         B10010000,
                         B10010000,
                         B10001000,
                         B01000100,
                         B00111100},
                        {B00111100,
                         B01000010,
                         B10000100,
                         B10011000,
                         B10011000,
                         B10000100,
                         B01000010,
                         B00111100},
                        {B00111100,
                         B01000010,
                         B10000011,
                         B10011100,
                         B10011100,
                         B10000011,
                         B01000010,
                         B00111100},
                        {B00111100,
                         B01000010,
                         B10000001,
                         B10011111,
                         B10011111,
                         B10000001,
                         B01000010,
                         B00111100}};
