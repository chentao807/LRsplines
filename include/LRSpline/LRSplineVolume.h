#ifndef LRSPLINEVOLUME_H
#define LRSPLINEVOLUME_H

#include <vector>
#include <GoTools/utils/Point.h>
#include <GoTools/geometry/Streamable.h>
#include <GoTools/trivariate/SplineVolume.h>
#include <boost/rational.hpp>
#include "HashSet.h"
#include "Basisfunction.h"
#include "LRSplineSurface.h"

namespace LR {

class Basisfunction;
class MeshRectangle;
class Element;

class LRSplineVolume : public Go::Streamable {

public:
	LRSplineVolume();
	LRSplineVolume(Go::SplineVolume *surf);
	LRSplineVolume(int n1, int n2, int n3, int order_u, int order_v, int order_w, double *knot_u, double *knot_v, double *knot_w, double *coef, int dim, bool rational=false);
	~LRSplineVolume();

	LRSplineVolume &operator=(LRSplineVolume& copythis);
	LRSplineVolume* copy();
	// surface evaluation
	virtual void point(Go::Point &pt, double u, double v, double w, int iEl=-1) const;
	virtual void point(Go::Point &pt, double u, double v, double w, int iEl, bool u_from_right, bool v_from_right, bool w_from_right) const;
	virtual void point(std::vector<Go::Point> &pts, double upar, double vpar, double wpar, int derivs, int iEl=-1) const;
	void computeBasis (double param_u, double param_v, double param_w, Go::BasisPtsSf     & result, int iEl=-1 ) const;
	void computeBasis (double param_u, double param_v, double param_w, Go::BasisDerivsSf  & result, int iEl=-1 ) const;
	void computeBasis (double param_u, double param_v, double param_w, Go::BasisDerivsSf2 & result, int iEl=-1 ) const;
	void computeBasis (double param_u,
	                   double param_v,
	                   double param_w,
	                   std::vector<std::vector<double> >& result,
	                   int derivs=0,
	                   int iEl=-1 ) const;
	int getElementContaining(double u, double v, double w) const;
	// TODO: get rid of the iEl argument in evaluation signatures - it's too easy to mess it up (especially with derivatives at multiple-knot boundaries). 
	//       Try and sort the Elements after all refinements and binary search for the containing point in logarithmic time

	// refinement functions
	/*
	void refineBasisFunction(int index);
	void refineBasisFunction(const std::vector<int> &indices);
	void refineElement(int index);
	void refineElement(const std::vector<int> &indices);
	void refineByDimensionIncrease(const std::vector<double> &error, double beta);
	*/

	// set refinement state parameters
	void setRefStrat(enum refinementStrategy strat) { refStrat_        = strat;    };
	void setRefSymmetry(int symmetry)               { this->symmetry_  = symmetry; };
	void setRefMultiplicity(int mult)               { refKnotlineMult_ = mult;     };
	void setMaxTjoints(int n)                       { maxTjoints_      = n;        };
	void setCloseGaps(bool doClose)                 { doCloseGaps_     = doClose;  };
	void setMaxAspectRatio(double r, bool aposterioriFix=true) {
		maxAspectRatio_ = r;
		doAspectRatioFix_ = aposterioriFix;
	}

	// (private) refinement functions
	/*
	MeshRectangle* insert_const_u_edge(double u, double start_v, double stop_v, int multiplicity=1);
	MeshRectangle* insert_const_v_edge(double v, double start_u, double stop_u, int multiplicity=1);
	void getFullspanLines(  int iEl,    std::vector<MeshRectangle*>& lines);
	void getMinspanLines(   int iEl,    std::vector<MeshRectangle*>& lines);
	void getStructMeshLines(int iBasis, std::vector<MeshRectangle*>& lines);
	void aPosterioriFixes() ;
	void closeGaps(            std::vector<MeshRectangle*>* newLines=NULL);
	void enforceMaxTjoints(    std::vector<MeshRectangle*>* newLines=NULL);
	void enforceMaxAspectRatio(std::vector<MeshRectangle*>* newLines=NULL);
	*/

	// linear independence methods
	bool isLinearIndepByOverloading(bool verbose) ;
	bool isLinearIndepByMappingMatrix(bool verbose) const ;
	bool isLinearIndepByFloatingPointMappingMatrix(bool verbose) const ;
	void getNullSpace(std::vector<std::vector<boost::rational<long long> > >& nullspace) const ;

	void updateSupport(Basisfunction *f) ;
	void updateSupport(Basisfunction *f,
	                   std::vector<Element*>::iterator start,
	                   std::vector<Element*>::iterator end ) ;
	
	void generateIDs() const;

	// common get methods
	void getGlobalKnotVector      (std::vector<double> &knot_u,
	                               std::vector<double> &knot_v,
	                               std::vector<double> &knot_w) const;
	void getGlobalKnotUniqueVector(std::vector<double> &knot_u,
	                               std::vector<double> &knot_v,
	                               std::vector<double> &knot_w) const;
	virtual double startparam_u() const                { return start_u_; };
	virtual double startparam_v() const                { return start_v_; };
	virtual double startparam_w() const                { return start_w_; };
	virtual double endparam_u()   const                { return end_u_; };
	virtual double endparam_v()   const                { return end_v_; };
	virtual double endparam_w()   const                { return end_w_; };
	virtual int dimension()       const                { return dim_; };
	int order_u()                 const                { return order_u_; };
	int order_v()                 const                { return order_v_; };
	int order_w()                 const                { return order_w_; };
	bool rational()               const                { return rational_; };
	int nBasisFunctions()         const                { return basis_.size(); };
	int nElements()               const                { return element_.size(); };
	int nMeshRectangles()         const                { return meshrect_.size(); };

	// more get-methods
	std::vector<MeshRectangle*>::iterator  meshrectBegin()         { return meshrect_.begin(); };
	std::vector<MeshRectangle*>::iterator  meshrectEnd()           { return meshrect_.end(); };
	std::vector<Element*>::iterator        elementBegin()          { return element_.begin(); };
	std::vector<Element*>::iterator        elementEnd()            { return element_.end(); };
	HashSet_iterator<Basisfunction*>       basisBegin()            { return basis_.begin(); };
	HashSet_iterator<Basisfunction*>       basisEnd()              { return basis_.end(); };
	HashSet_const_iterator<Basisfunction*> basisBegin()   const    { return basis_.begin(); };
	HashSet_const_iterator<Basisfunction*> basisEnd()     const    { return basis_.end(); };
	const HashSet<Basisfunction*>& getAllBasisfunctions() const    { return basis_ ;};
	Element* getElement(int i)                                     { return element_[i]; };
	MeshRectangle* getMeshRectangle(int i)                         { return meshrect_[i]; };
	Basisfunction* getBasisfunction(int iBasis) {
		HashSet_iterator<Basisfunction*> it = basis_.begin();
		for(int i=0; i<iBasis; ++i) ++it;
		return *it;
	}
	void getEdgeFunctions(std::vector<Basisfunction*> &edgeFunctions, parameterEdge edge, int depth=1) const;
	void getBezierElement(int iEl, std::vector<double> &controlPoints) const;

	// assorted specialized functions
	void rebuildDimension(int dimvalue) ;
	void getDiagonalElements(      std::vector<int> &result) const ;
	void getDiagonalBasisfunctions(std::vector<Basisfunction*> &result) const ;
	void printElements(std::ostream &out) const;

	// interpolate and approximate functions
	/*
	void getLeastSquaresEdge(double (*f)(double, double),
	                         parameterEdge edge,
	                         std::vector<int> id,
	                         std::vector<double> val) const;
	*/

	// input output methods
	virtual void read(std::istream &is);
	virtual void write(std::ostream &os) const;

	MeshRectangle* insert_line(MeshRectangle *newRect) ;

private:
	void split(int constDir, Basisfunction *b, double new_knot, int multiplicity, HashSet<Basisfunction*> &newFunctions);
	
	bool rational_;
	HashSet<Basisfunction*> basis_;
	std::vector<Basisfunction*> basisVector; // only used in read/write functions
	std::vector<MeshRectangle*> meshrect_;
	std::vector<Element*> element_;
	int dim_;
	int order_u_;
	int order_v_;
	int order_w_;
	double start_u_;
	double start_v_;
	double start_w_;
	double end_u_;
	double end_v_;
	double end_w_;

	// refinement parameters
	enum refinementStrategy refStrat_;
	int                     refKnotlineMult_;
	int                     symmetry_;
	int                     maxTjoints_;
	bool                    doCloseGaps_;
	bool                    doAspectRatioFix_;
	double                  maxAspectRatio_;

};

} // end namespace LR

#endif

