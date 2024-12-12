#include "faces.h"

// Define face bitmaps here or include externally
const uint8_t happyFace[] = { /* Bitmap data */ };
const uint8_t sadFace[] = { /* Bitmap data */ };
const uint8_t angryFace[] = { /* Bitmap data */ };
const uint8_t surprisedFace[] = { /* Bitmap data */ };
const uint8_t neutralFace[] = { /* Bitmap data */ };

// Array of all faces
static const uint16_t* faces[] = {Basic_face, Calm_face, engy_face, Huh_face, Love_face, Rly_face, shy_face};
static const char* faceNames[] = {"Basic_face", "Calm_face", "engy_face", "Huh_face", "Love_face", "Rly_face", "shy_face"};
static uint16_t currentFaceIndex = 4;

void initFaces() {
    currentFaceIndex = 1; // Default to the first face
}

const uint16_t* getCurrentFace() {
    return faces[currentFaceIndex];
}
const char* getcurrentFaceName(){
    return faceNames[currentFaceIndex];
}
void setFace(uint16_t index) {
    if (index < (sizeof(faces) / sizeof(faces[0]))) {
        currentFaceIndex = index;
    }
}
const uint16_t* currentFaceBitmap = faces[currentFaceIndex]; // Pointer to the currently displayed face bitmap
const char* currentFaceName = faceNames[currentFaceIndex]; // Name of the current face

