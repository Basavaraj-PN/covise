#include "coVR3DTransGizmo.h"
#include <OpenVRUI/osg/mathUtils.h>
#include <osg/MatrixTransform>
#include <cover/coVRNavigationManager.h>
#include <osg/ShapeDrawable>
#include <osg/Geometry>
#include <cover/VRSceneGraph.h>
#include <cover/coVRConfig.h>
#include <cover/coVRPluginSupport.h>
#include <osg/io_utils>
#include <vrbclient/SharedState.h>


using namespace opencover;

coVR3DTransGizmo::coVR3DTransGizmo(osg::Matrix m, float s, coInteraction::InteractionType type, const char *iconName, const char *interactorName, coInteraction::InteractionPriority priority = Medium,coVR3DGizmo* gizmoPointer)
    : coVR3DGizmoType(m, s, type, iconName, interactorName, priority, gizmoPointer) 
{
    if (cover->debugLevel(2))
    {
        fprintf(stderr, "new coVR3DTransGizmo(%s)\n", interactorName);
    }

    createGeometry();

}

coVR3DTransGizmo::~coVR3DTransGizmo()
{
    if (cover->debugLevel(2))
        fprintf(stderr, "\ndelete ~coVR3DTransGizmo\n");

}

void
coVR3DTransGizmo::createGeometry()
{
   if (cover->debugLevel(4))
        fprintf(stderr, "\ncoVR3DTransGizmo::createGeometry\n");


    osg::ShapeDrawable *sphereDrawable, *xCylDrawable, *yCylDrawable, *zCylDrawable, *xyPlaneDrawable,*xzPlaneDrawable,*yzPlaneDrawable;

    osg::Vec3 origin(0, 0, 0), px(1, 0, 0), py(0, 1, 0), pz(0, 0, 1);
    osg::Vec3 yaxis(0, 1, 0);

    axisTransform = new osg::MatrixTransform();
    axisTransform->setStateSet(VRSceneGraph::instance()->loadDefaultGeostate(osg::Material::AMBIENT_AND_DIFFUSE));
    geometryNode = axisTransform.get();
    scaleTransform->addChild(geometryNode.get());

    osg::Sphere *mySphere = new osg::Sphere(origin, 0.5);
    osg::TessellationHints *hint = new osg::TessellationHints();
    hint->setDetailRatio(0.5);
    sphereDrawable = new osg::ShapeDrawable(mySphere, hint);
    sphereDrawable->setColor(_grey);
    sphereGeode = new osg::Geode();
    sphereGeode->addDrawable(sphereDrawable);
    axisTransform->addChild(sphereGeode.get());

    //create axis
    auto cyl = new osg::Cylinder(origin, 0.15, _arrowLength);
    xCylDrawable = new osg::ShapeDrawable(cyl, hint);
    yCylDrawable = new osg::ShapeDrawable(cyl, hint);
    zCylDrawable = new osg::ShapeDrawable(cyl, hint);

    xCylDrawable->setColor(_red);
    yCylDrawable->setColor(_green);
    zCylDrawable->setColor(_blue);

    xAxisTransform = new osg::MatrixTransform();
    xAxisTransform->setMatrix(osg::Matrix::rotate(osg::inDegrees(90.), 0, 1, 0)*osg::Matrix::translate(osg::Vec3(_arrowLength, 0, 0)*0.5));
    yAxisTransform = new osg::MatrixTransform();
    yAxisTransform->setMatrix(osg::Matrix::rotate(osg::inDegrees(-90.), 1, 0, 0)*osg::Matrix::translate(osg::Vec3(0, _arrowLength, 0)*0.5));
    zAxisTransform = new osg::MatrixTransform();
    zAxisTransform->setMatrix(osg::Matrix::translate(osg::Vec3(0, 0, _arrowLength)*0.5));

    translateXaxisGeode = new osg::Geode;
    translateXaxisGeode->addDrawable(xCylDrawable);
    xAxisTransform->addChild(translateXaxisGeode);
    translateYaxisGeode = new osg::Geode;
    translateYaxisGeode->addDrawable(yCylDrawable);
    yAxisTransform->addChild(translateYaxisGeode);
    translateZaxisGeode = new osg::Geode;
    translateZaxisGeode->addDrawable(zCylDrawable);
    zAxisTransform->addChild(translateZaxisGeode);

    axisTransform->addChild(xAxisTransform);
    axisTransform->addChild(yAxisTransform);
    axisTransform->addChild(zAxisTransform);
    
    //create planes
    auto plane = new osg::Box(origin,_arrowLength/3, 0.1, _arrowLength/3);
    xzPlaneDrawable = new osg::ShapeDrawable(plane, hint);
    xyPlaneDrawable = new osg::ShapeDrawable(plane, hint);
    yzPlaneDrawable = new osg::ShapeDrawable(plane, hint);

    xyPlaneDrawable->setColor(_grey);
    xzPlaneDrawable->setColor(_grey);
    yzPlaneDrawable->setColor(_grey);

    xzPlaneTransform = new osg::MatrixTransform();
    xzPlaneTransform->setMatrix(osg::Matrix::translate(osg::Vec3(_arrowLength/3, 0, _arrowLength/3)));
    xyPlaneTransform = new osg::MatrixTransform();
    xyPlaneTransform->setMatrix(osg::Matrix::rotate(osg::inDegrees(-90.), 1, 0, 0)*osg::Matrix::translate(osg::Vec3(_arrowLength/3, _arrowLength/3, 0)));
    yzPlaneTransform = new osg::MatrixTransform();
    yzPlaneTransform->setMatrix(osg::Matrix::rotate(osg::inDegrees(-90.), 0, 0, 1)*osg::Matrix::translate(osg::Vec3(0, _arrowLength/3, _arrowLength/3)));

    translateXZplaneGeode = new osg::Geode;
    translateXZplaneGeode->addDrawable(xzPlaneDrawable);
    xzPlaneTransform->addChild(translateXZplaneGeode);
    translateXYplaneGeode = new osg::Geode;
    translateXYplaneGeode->addDrawable(xyPlaneDrawable);
    xyPlaneTransform->addChild(translateXYplaneGeode);
    translateYZplaneGeode = new osg::Geode;
    translateYZplaneGeode->addDrawable(yzPlaneDrawable);
    yzPlaneTransform->addChild(translateYZplaneGeode);

    axisTransform->addChild(xzPlaneTransform);
    axisTransform->addChild(xyPlaneTransform);
    axisTransform->addChild(yzPlaneTransform);

}

void coVR3DTransGizmo::startInteraction()
{
    if (cover->debugLevel(5))
        fprintf(stderr, "\ncoVR3DTransGizmo::startInteraction\n");

    _translateXonly = _hitNode == translateXaxisGeode;
    _translateYonly = _hitNode == translateYaxisGeode;
    _translateZonly = _hitNode == translateZaxisGeode;
    _translateXYonly = _hitNode == translateXYplaneGeode;
    _translateXZonly = _hitNode == translateXZplaneGeode;
    _translateYZonly = _hitNode == translateYZplaneGeode;

   
    /* wie setze ich das hier um, brauch man das ? ###################################
    if (!_rotateOnly && !_translateOnly)
    {
        _translateOnly = is2D();
    }
    */

    coVR3DGizmoType::startInteraction();
    
}
void coVR3DTransGizmo::doInteraction()
{

    if (cover->debugLevel(5))
        fprintf(stderr, "\ncoVR3DTransGizmo::move\n");
    
    osg::Vec3 lp0_o, lp1_o, pointerDir_o;
    calculatePointerDirection_o(lp0_o, lp1_o, pointerDir_o);

    /*  check if necessary !!! Achtung an richtiger Stelle platzieren, da currHandMat modifiziert wird! 

    // forbid translation in y-direction if traverseInteractors is on ############## wozu brauche ich das ? 
    if (coVRNavigationManager::instance()->getMode() == coVRNavigationManager::TraverseInteractors && coVRConfig::instance()->useWiiNavigationVisenso())
    {
        osg::Vec3 trans = currHandMat.getTrans();
        trans[1] = _oldHandMat.getTrans()[1];
        currHandMat.setTrans(trans);
    }

    */

    if(_translateXonly)
        _interMat_o.setTrans(calculatePointOfShortestDistance(lp0_o, lp1_o, osg::X_AXIS)); 

    else if(_translateYonly)
        _interMat_o.setTrans(calculatePointOfShortestDistance(lp0_o, lp1_o, osg::Y_AXIS)); 

    else if(_translateZonly)
        _interMat_o.setTrans(calculatePointOfShortestDistance(lp0_o, lp1_o, osg::Z_AXIS)); 

    else if(_translateXYonly)
        _interMat_o.setTrans(calcPlaneLineIntersection(lp0_o, lp1_o, osg::Z_AXIS)); 

    else if(_translateXZonly)
        _interMat_o.setTrans(calcPlaneLineIntersection(lp0_o, lp1_o, osg::Y_AXIS)); 

    else if(_translateYZonly)
        _interMat_o.setTrans(calcPlaneLineIntersection(lp0_o, lp1_o, osg::X_AXIS)); 

    else // allow translation in all directions 
    {
        osg::Matrix w_to_o = cover->getInvBaseMat();
        osg::Matrix currHandMat = getPointerMat();
        osg::Matrix currHandMat_o = currHandMat * w_to_o;
        osg::Vec3 currHandPos_o = currHandMat_o.getTrans();  
        osg::Vec3 interPos = currHandPos_o + pointerDir_o * _distance + _diff;

        _interMat_o.setTrans(interPos); 
    }


    if (cover->restrictOn())
    {
        // restrict to visible scene
        osg::Vec3 pos_o, restrictedPos_o;
        pos_o = _interMat_o.getTrans();
        restrictedPos_o = restrictToVisibleScene(pos_o);
        _interMat_o.setTrans(restrictedPos_o);
    }

    // save old transformation
     _oldInterMat_o = _interMat_o;

    // and now we apply it
    updateTransform(_interMat_o);
}


osg::Vec3 coVR3DTransGizmo::calculatePointOfShortestDistance(const osg::Vec3& lp0, const osg::Vec3& lp1, osg::Vec3 axis_o) const
{
    osg::Vec3 newPos, pointLine1, pointLine2;

    _helperLine->update(osg::Vec3(0,0,0)*getMatrix(),  axis_o*getMatrix());
    // if(_helperLine->getPointsOfShortestDistance(lp0, lp1, pointLine1, pointLine2))  what happens if lines are parallel ? 
    // {
        _helperLine->getPointsOfShortestDistance(lp0, lp1, pointLine1, pointLine2);
        newPos = pointLine1 + _diff;
        if(axis_o == osg::X_AXIS)
        {
            newPos.z() = _startInterMat_o.getTrans().z();
            newPos.y() = _startInterMat_o.getTrans().y();   
        }
        else if(axis_o == osg::Y_AXIS)
        {
            newPos.z() = _startInterMat_o.getTrans().z();
            newPos.x() = _startInterMat_o.getTrans().x();
        }
        else if(axis_o == osg::Z_AXIS)
        {
            newPos.x() = _startInterMat_o.getTrans().x();
            newPos.y() = _startInterMat_o.getTrans().y();
        }
    // }
    // else
        // newPos = _oldInteractorXformMat_o.getTrans();
        
    return newPos;
}








