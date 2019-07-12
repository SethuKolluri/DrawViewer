/******************************************************************************
**  dwg2dxf - Program to convert dwg/dxf to dxf(ascii & binary)              **
**                                                                           **
**  Copyright (C) 2015 José F. Soriano, rallazz@gmail.com                    **
**                                                                           **
**  This library is free software, licensed under the terms of the GNU       **
**  General Public License as published by the Free Software Foundation,     **
**  either version 2 of the License, or (at your option) any later version.  **
**  You should have received a copy of the GNU General Public License        **
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.    **
******************************************************************************/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>

#include <string>
#include "../Headers/dx_iface.h"

#include "../../../libdxfrw-master/src/libdwgr.h"
#include "../../../libdxfrw-master/src/libdxfrw.h"


bool dx_iface::dumpData(const std::string& fileI, dx_data *fData) {
    unsigned int found = fileI.find_last_of(".");
    std::string fileExt = fileI.substr(found + 1);
    std::transform(fileExt.begin(), fileExt.end(), fileExt.begin(), ::toupper);
    cData = fData;
    currentBlock = cData->mBlock;

    bool success = false;
    if (fileExt == "DXF") {
        //loads dxf
        dxfRW* dxf = new dxfRW(fileI.c_str());
        success = dxf->read(this, false);
        delete dxf;
    }
    else if (fileExt == "DWG") {
        //loads dwg
        dwgR* dwg = new dwgR(fileI.c_str());
        success = dwg->read(this, false);
        delete dwg;
    }

    if (success) {
        std::ofstream ofs("out.json");
        if (!ofs.is_open())
        {
            std::cout << "Failed to open out.json file" << std::endl;
        };
        picojson::value jsonValue;
        picojson::object full;
        picojson::array layers;

        std::unordered_map<std::string, picojson::array> curves;

        for (std::list<DRW_Entity*>::const_iterator it =
            cData->mBlock->ent.begin(); it != cData->mBlock->ent.end();
            ++it) {
            DRW_Entity* e = *it;

            picojson::object data;
            switch (e->eType) {
            case DRW::LINE:
                data = picojson::object();
                dumpLine(static_cast<DRW_Line*>(e), data);
                curves[e->layer].push_back(picojson::value(data));
                break;
            case DRW::ARC:
                data = picojson::object();
                dumpArc(static_cast<DRW_Arc*>(e), data);
                curves[e->layer].push_back(picojson::value(data));
                break;
            case DRW::CIRCLE:
                data = picojson::object();
                dumpCircle(static_cast<DRW_Circle*>(e), data);
                curves[e->layer].push_back(picojson::value(data));
                break;
            case DRW::POLYLINE:
                data = picojson::object();
                dumpPolyLine(static_cast<DRW_Polyline*>(e), data);
                curves[e->layer].push_back(picojson::value(data));
                break;
            case DRW::LWPOLYLINE:
                data = picojson::object();
                dumpPolyLine(static_cast<DRW_LWPolyline*>(e), data);
                curves[e->layer].push_back(picojson::value(data));
                break;
            default:
                break;
            }
        }

        for (auto layerData : curves)
        {
            picojson::object layer;
            layer["name"] = picojson::value(layerData.first);
            layer["curves"] = picojson::value(layerData.second);
            layers.push_back(picojson::value(layer));
        }

        full["layers"] = picojson::value(layers);
        jsonValue = picojson::value(full);
        ofs << jsonValue.serialize().c_str();
        ofs.close();
    }
    return success;
}

void dx_iface::writeEntity(DRW_Entity* e) {
    switch (e->eType) {
    case DRW::POINT:
        dxfW->writePoint(static_cast<DRW_Point*>(e));
        break;
    case DRW::LINE:
        dxfW->writeLine(static_cast<DRW_Line*>(e));
        break;
    case DRW::CIRCLE:
        dxfW->writeCircle(static_cast<DRW_Circle*>(e));
        break;
    case DRW::ARC:
        dxfW->writeArc(static_cast<DRW_Arc*>(e));
        break;
    case DRW::SOLID:
        dxfW->writeSolid(static_cast<DRW_Solid*>(e));
        break;
    case DRW::ELLIPSE:
        dxfW->writeEllipse(static_cast<DRW_Ellipse*>(e));
        break;
    case DRW::LWPOLYLINE:
        dxfW->writeLWPolyline(static_cast<DRW_LWPolyline*>(e));
        break;
    case DRW::POLYLINE:
        dxfW->writePolyline(static_cast<DRW_Polyline*>(e));
        break;
    case DRW::SPLINE:
        dxfW->writeSpline(static_cast<DRW_Spline*>(e));
        break;
        //    case RS2::EntitySplinePoints:
        //        writeSplinePoints(static_cast<DRW_Point*>(e));
        //        break;
        //    case RS2::EntityVertex:
        //        break;
    case DRW::INSERT:
        dxfW->writeInsert(static_cast<DRW_Insert*>(e));
        break;
    case DRW::MTEXT:
        dxfW->writeMText(static_cast<DRW_MText*>(e));
        break;
    case DRW::TEXT:
        dxfW->writeText(static_cast<DRW_Text*>(e));
        break;
    case DRW::DIMLINEAR:
    case DRW::DIMALIGNED:
    case DRW::DIMANGULAR:
    case DRW::DIMANGULAR3P:
    case DRW::DIMRADIAL:
    case DRW::DIMDIAMETRIC:
    case DRW::DIMORDINATE:
        dxfW->writeDimension(static_cast<DRW_Dimension*>(e));
        break;
    case DRW::LEADER:
        dxfW->writeLeader(static_cast<DRW_Leader*>(e));
        break;
    case DRW::HATCH:
        dxfW->writeHatch(static_cast<DRW_Hatch*>(e));
        break;
    case DRW::IMAGE:
        dxfW->writeImage(static_cast<DRW_Image*>(e), static_cast<dx_ifaceImg*>(e)->path);
        break;
    default:
        break;
    }
}



//MY CODE STARTS

void dx_iface::dumpLine(DRW_Line* line, picojson::object& data)
{
    if (nullptr == line)
        return;

    //initialize data
    picojson::object geometry;
    picojson::array start_point;
    picojson::array end_point;


    //populate data
    start_point.push_back(picojson::value(line->basePoint.x));
    start_point.push_back(picojson::value(line->basePoint.y));
    start_point.push_back(picojson::value(line->basePoint.z));
    end_point.push_back(picojson::value(line->secPoint.x));
    end_point.push_back(picojson::value(line->secPoint.y));
    end_point.push_back(picojson::value(line->secPoint.z));

    geometry["start_point"] = picojson::value(start_point);
    geometry["end_point"] = picojson::value(end_point);

    //dump data
    data["type"] = picojson::value("straight");
    data["geometry"] = picojson::value(geometry);
}


void dx_iface::dumpArc(DRW_Arc* arc, picojson::object& data)
{
    if (nullptr == arc)
        return;
    //initialize data
    picojson::object geometry;
    picojson::array center;

    //populate data
    center.push_back(picojson::value(arc->center().x));
    center.push_back(picojson::value(arc->center().y));
    center.push_back(picojson::value(arc->center().z));

    geometry["center"] = picojson::value(center);
    geometry["radius"] = picojson::value(arc->radious);
    geometry["start_param"] = picojson::value(arc->staangle);
    geometry["end_param"] = picojson::value(arc->endangle);

    //dumpData
    data["geometry"] = picojson::value(geometry);

    data["type"] = picojson::value("circle");
}

void dx_iface::dumpCircle(DRW_Circle* arc, picojson::object& data)
{
    if (nullptr == arc)
        return;
    //initialize data
    picojson::object geometry;
    picojson::array center;

    //populate data
    center.push_back(picojson::value(arc->basePoint.x));
    center.push_back(picojson::value(arc->basePoint.y));
    center.push_back(picojson::value(arc->basePoint.z));

    geometry["center"] = picojson::value(center);
    geometry["radius"] = picojson::value(arc->radious);
    geometry["start_param"] = picojson::value(0,0);
    geometry["end_param"] = picojson::value(M_PIx2);

    //dumpData
    data["geometry"] = picojson::value(geometry);

    data["type"] = picojson::value("circle");
}
void dx_iface::dumpPolyLine(DRW_Polyline* polyLine, picojson::object& data)
{
    if (nullptr == polyLine)
        return;
    //initialize data
    picojson::object geometry;
    picojson::array points;

    //populate data   
    for (auto vertex : polyLine->vertlist)
    {
        picojson::array point;
        point.push_back(picojson::value(vertex->basePoint.x));
        point.push_back(picojson::value(vertex->basePoint.y));
        point.push_back(picojson::value(vertex->basePoint.z));
        points.push_back(picojson::value(point));
    }
    geometry["num_points"] = picojson::value(double(polyLine->vertexcount));
    geometry["points"] = picojson::value(points);

    //dumpData
    data["geometry"] = picojson::value(geometry);
    data["type"] = picojson::value("polyline");
}

void dx_iface::dumpPolyLine(DRW_LWPolyline* polyLine, picojson::object& data)
{
    if (nullptr == polyLine)
        return;
    //initialize data
    picojson::object geometry;
    picojson::array points;

    //populate data   
    for (auto vertex : polyLine->vertlist)
    {
        picojson::array point;
        point.push_back(picojson::value(vertex->x));
        point.push_back(picojson::value(vertex->y));
        point.push_back(picojson::value(0.0));
        points.push_back(picojson::value(point));
    }
    geometry["num_points"] = picojson::value(double(polyLine->vertexnum));
    geometry["points"] = picojson::value(points);

    //dumpData
    data["geometry"] = picojson::value(geometry);
    data["type"] = picojson::value("polyline");
}