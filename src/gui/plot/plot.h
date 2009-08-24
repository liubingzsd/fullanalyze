/***********************************************************************

This file is part of the FullAnalyze project source files.

FullAnalyze is an open source software for handling, visualizing and 
processing lidar data (waveforms and point clouds). FullAnalyze is 
distributed under the CeCILL-B licence. See Licence_CeCILL-B_V1-en.txt 
or http://www.cecill.info for more details.


Homepage: 

	http://fullanalyze.sourceforge.net
	
Copyright:
	
	Institut Geographique National & CEMAGREF (2009)

Author: 

	Adrien Chauve
	
	

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.
 
***********************************************************************/

#ifndef PLOT_H
#define PLOT_H

/*! \class Plot
 *  \brief Fenetre graphique qui affiche un plot 1D
 *
 *  Classe fenetre wx qui affiche un plot 1D, avec fenetre de gestion de calques et de config.
 *
 *
 */

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "wx/wx.h"
#include "wx/minifram.h"
#include "wx/clrpicker.h"




#include "plottype.h"

class PlotConfig : public wxMiniFrame
{
	public:

		PlotConfig(wxWindow* parent, const wxString& title);

		//fonctions liées à des événements
		void OnClickQuitter(wxCloseEvent& event);
		void OnResize(wxSizeEvent& event);

		//Appliquer les changements sans fermer la fenêtre de config
		void OnClickApply();

		//Accesseurs de la config
		const double GetMinX() const;
		const double GetMaxX() const;
		const double GetMinY() const;
		const double GetMaxY() const;

		//Fonctions utiles
		void RecalculeMinMaxY();

		DECLARE_EVENT_TABLE()

	private:
		//Contrôles
		wxFlexGridSizer* m_Sizer;

		wxWindow* m_parent;

		enum
		{
			ID_ToolBar_ZoomAll,
			ID_ToolBar_SaveAs,
			ID_ToolBar_Notepad,
			ID_ToolBar,
		};

};

//type de base pour configurer un calque
class ElementConfigCalque : public wxWindow
{
	public:
		wxCheckBoxBase* m_checkBoxAfficheTrait;
		wxCheckBoxBase* m_checkBoxAffichePoints;
		wxColourPickerCtrl* m_ColourPickerCtrlCouleur;
		wxTextCtrl* m_textCtrlMinValueAttribut;
		wxTextCtrl* m_textCtrlMaxValueAttribut;
};

class PlotCalques : public wxMiniFrame
{
	public:

		PlotCalques(wxWindow* parent, const shared_ptr<PlotType>& typePlot, const wxString& title);

		//Type definissant un tableau de controle pour configurer les calques
		typedef ElementConfigCalque* ptrElementConfigCalque;
		typedef std::map<std::string, ptrElementConfigCalque> t_MapConfigCalques;

		//fonctions liées à des événements
		void OnClickQuitter(wxCloseEvent& event);
		void OnResize(wxSizeEvent& event);

		void OnCheckBox(wxCommandEvent& event);
		void OnPickColour(wxColourPickerEvent& event);
		void OnTextChange(wxCommandEvent& event);
		//    void OnScrollSlider(wxScrollEvent& event);

		//Accesseurs de la config
		const wxColour GetColour(const std::string& dataName);
		const double GetMinValue(const std::string& dataName);
		const double GetMaxValue(const std::string& dataName);
		const bool AfficheCourbe(const std::string& dataName);
		const bool AffichePoints(const std::string& dataName);

		void UpdateTableauCalques();

		DECLARE_EVENT_TABLE()

		private:
		//Contrôles
		wxFlexGridSizer* m_Sizer;
		t_MapConfigCalques m_configCalque;

		wxWindow* m_parent;
		shared_ptr<PlotType> m_plotType;

	};

//class Plot: public wxFrame
class Plot : public wxPanel
{
	public:

		//constructeurs
		Plot(wxWindow* parent, const shared_ptr<PlotType>& typePlot, const wxString& title = _("PlotWindow"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
		//    Plot(PlotType* typePlot);
		virtual ~Plot();

		// Redimensionnement de la fenetre de dessin
		void OnResize(wxSizeEvent& event);
		// Methode surchargee dans laquelle se trouve tout ce qu'on veut afficher
		void OnPaint(wxPaintEvent &event);
		// Evenements quand la souris bouge...

		void OnClickCalques(wxCommandEvent& event);
		void OnClickSaveAs(wxCommandEvent& event);
		void OnClickZoomAll(wxCommandEvent& event);

		//void OnKeyDown(wxKeyEvent & event);
		void OnMouse(wxMouseEvent& event);

		// Rafraichissement de la fenetre de dessin
		void Redraw();

		const t_ptrConstPlotDataContainer& GetDataPlotContainer()
		{
			return m_dataPlot;
		}

	private:
		//controles de la fenetre
		PlotCalques* m_frameConfigCalques;
		//    wxToolBar* m_ToolBar;
		wxBoxSizer* m_Sizer;

		// Largeur & hauteur de la frame de profils
		wxSize m_taille_fenetre;

		//data a plotter
		t_ptrConstPlotDataContainer m_dataPlot;
		shared_ptr<PlotType> m_plotType;

		//Fonctions de conversion de coordonnées fenêtre de plot / données
		bool data2plot(double& result, const double c, const double c_min, const double c_max, const double longueur_axe)
		{
			if(c>=c_min && c<=c_max)
			{
				result = (c-c_min)/(c_max-c_min)*longueur_axe;
				return true;
			}
			return false;
		}
		double plot2data(const double c, const double c_min, const double c_max, const double longueur_axe)
		{
			return c_min+(c_max-c_min)*c/longueur_axe;
		}

		DECLARE_EVENT_TABLE()
	};




#endif // PLOT_H
