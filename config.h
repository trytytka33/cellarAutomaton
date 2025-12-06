#ifndef FONTPATHS_H
#define FONTPATHS_H

#include <string>

// na linux dziala :)
#ifdef _WIN32
    const std::string fontPathArial = "C:/Windows/Fonts/arial.ttf";
    const std::string fontPathHelvetica = "C:/Windows/Fonts/arial.ttf";
#elif __APPLE__
    const std::string fontPathArial = "/System/Library/Fonts/Supplemental/Arial.ttf";
    const std::string fontPathHelvetica = "/System/Library/Fonts/Helvetica.ttc";
#else  // Linux
    const std::string fontPathArial = "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf";
    const std::string fontPathHelvetica = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
#endif

#endif // FONTPATHS_H