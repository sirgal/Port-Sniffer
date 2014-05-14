#include "ports/gui_builders/port_guibuilder.h"


void PortGuiBuilder::cleanLayout(QFormLayout *layout)
{
    while( layout->layout()->count() != 0 )
        layout->takeAt(0);
}
