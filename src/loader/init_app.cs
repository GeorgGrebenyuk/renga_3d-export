using Renga;
using System.Runtime.InteropServices;
using System;

namespace loader
{
    public static class LibraryExporter
    {

    } 
    public class init_app : Renga.IPlugin
    {
        //User_selections
        /// <summary>
        /// Учитывать или нет скрытые пользователем объекты на виде
        /// </summary>
        public static bool use_hidded_objects = false;
        /// <summary>
        /// Ограничение на максимальное количество граней в объекте. -1 = не использовать этот параметр
        /// </summary>
        public static int max_triangles_count = -1;
        /// <summary>
        /// Пересчитывать ли координаты геометрии по настроенным в Параметрах проекта значениям
        /// </summary>
        public static bool use_recalc_coordinates = false;
        /// <summary>
        /// Режим работы с геометрией - 3д объекты или сетки с материалами
        /// </summary>
        public static int export_mode = 0;
        /// <summary>
        /// Какой формат был выбран для экспорта
        /// </summary>
        public static int export_format = 0;

        [DllImport("exporter", CallingConvention = CallingConvention.StdCall, ExactSpelling = false,
    EntryPoint = "run_exporter")]
        private static extern void run_exporter(bool use_hidden, int use_max_triangles,
    bool recalc, int export_mode, int export_format);

        Renga.ActionEventSource follow_action;
        public bool Initialize(string pluginFolder)
        {
            Renga.Application renga_app = new Renga.Application();
            Renga.IUI renga_ui = renga_app.UI;
            Renga.IUIPanelExtension panel = renga_ui.CreateUIPanelExtension();

            Renga.IAction our_button = renga_ui.CreateAction();
            our_button.ToolTip = "Запустить настройки для экспорта";

            Renga.IImage icon = renga_ui.CreateImage();
            icon.LoadFromFile(pluginFolder + "\\logo.png");
            our_button.Icon = icon;

            follow_action = new ActionEventSource(our_button);
            follow_action.Triggered += (sender, args) =>
            {
                ConfigureTool window = new ConfigureTool();
                System.Windows.Forms.Application.Run(window);
                window.Close();
                run_exporter(use_hidded_objects, max_triangles_count, use_recalc_coordinates, export_mode, export_format);
            };

            panel.AddToolButton(our_button);
            renga_ui.AddExtensionToPrimaryPanel(panel);

            return true;
        }
        public void Stop()
        {
            follow_action.Dispose();
        }
    }
}
