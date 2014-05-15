#include "ports/gui_builders/port_guibuilder.h"


void PortGuiBuilder::cleanLayout(QFormLayout *layout)
{
    while( layout->layout()->count() != 0 ) {
        auto to_delete = layout->takeAt(0);
        delete to_delete->widget();
        delete to_delete;
    }
    layout->update();
}
