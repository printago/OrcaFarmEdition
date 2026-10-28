#ifndef slic3r_PrinterWebView_hpp_
#define slic3r_PrinterWebView_hpp_


#include "wx/artprov.h"
#include "wx/cmdline.h"
#include "wx/notifmsg.h"
#include "wx/settings.h"
#include <wx/setup.h>
#include <wx/event.h>
#include <wx/webview.h>
#include <wx/string.h>

#if wxUSE_WEBVIEW_EDGE
#include "wx/msw/webview_edge.h"
#endif

#include "wx/webviewarchivehandler.h"
#include "wx/webviewfshandler.h"
#include "wx/numdlg.h"
#include "wx/infobar.h"
#include "wx/filesys.h"
#include "wx/fs_arc.h"
#include "wx/fs_mem.h"
#include "wx/stdpaths.h"
#include <wx/panel.h>
#include <wx/tbarbase.h>
#include "wx/textctrl.h"
#include <wx/timer.h>
#include <functional>
#include <memory>
#include "Lazy.hpp"
#include "libslic3r/PrintConfig.hpp"


namespace Slic3r {
namespace GUI {

class PrinterWebViewHandler;
class PrintagoTabBridge;
class Plater;


class PrinterWebView : public wxPanel, public LazyInstance<PrinterWebView> {
public:
    PrinterWebView(wxWindow *parent);
    virtual ~PrinterWebView();

    // Attach the Printago "Edit in Orca Slicer" bridge to this webview (used only for the Printago
    // app tab). Registers the printago message channel and routes its messages to the bridge. Must be
    // called before the app URL is loaded; a no-op if already attached.
    void enable_printago_bridge(Plater* plater);

    void load_url(wxString& url, wxString apikey = "");

    // Navigate immediately, bypassing load_url()'s "park the URL until the panel is shown" path.
    // That deferred path is racy: OnLoaded() clears m_url_deferred on any successful load, including
    // the webview's initial about:blank, which can wipe a pending URL and leave the view blank
    // forever. Used by the Printago tab, which must load reliably on first show.
    void load_url_now(const wxString& url);
    void UpdateState();
    void OnClose(wxCloseEvent& evt);
    void OnError(wxWebViewEvent& evt);
    void OnLoaded(wxWebViewEvent& evt);
    void OnNewWindow(wxWebViewEvent& evt);
    void OnNavigating(wxWebViewEvent& evt);
    void OnScriptMessage(wxWebViewEvent& evt);
    void reload();
    void update_mode();

    // Optional hook: called on every navigation with the target URL. If it returns true the
    // navigation is vetoed (used to intercept the orcaslicer:// auth callback). Inert when unset.
    void set_navigation_intercept(std::function<bool(const wxString& url)> fn) { m_nav_intercept = std::move(fn); }

    // Override the webview's User-Agent (must be called before the page loads). Needed for the
    // Printago tab: the default BBL-Slicer UA omits the Safari/Version tokens on macOS, which makes
    // modern web apps (app.printago.io) render blank. Other webviews keep the default UA.
    void set_user_agent(const wxString& ua) { m_user_agent = ua; if (m_browser) m_browser->SetUserAgent(ua); }

    bool Show(bool show = true) override;

private:
    friend class PrinterWebViewHandler;

    void SendAPIKey();
    void create_browser();
    void reset_browser();

    wxWebView* m_browser;
    long m_zoomFactor;
    wxString m_apikey;
    bool m_apikey_sent;
    // Last url passed to load_url(), reloaded after reset_browser().
    wxString m_url;
    wxString m_url_deferred;
    // Host type the page was loaded for; it selects the handler, so a change needs a reload.
    PrintHostType m_host_type{htOctoPrint};
    // True once the page for m_url has finished loading in m_browser.
    bool m_url_loaded{false};
    std::function<bool(const wxString& url)> m_nav_intercept;
    // User-Agent override, re-applied when reset_browser() recreates the view.
    wxString m_user_agent;
    std::unique_ptr<PrinterWebViewHandler> m_handler;
    // Printago tab only: the bridge and the plater it was attached with, so reset_browser() can
    // rebuild the bridge on the new view.
    std::unique_ptr<PrintagoTabBridge> m_printago_bridge;
    Plater* m_printago_plater{nullptr};
    bool m_reset_on_show{false};

    // DECLARE_EVENT_TABLE()
};

} // GUI
} // Slic3r

#endif /* slic3r_Tab_hpp_ */
