using Renga;
using System.Runtime.InteropServices;
using System;

namespace loader
{
    public class init_app : Renga.IPlugin
    {
        //User_selections
        Renga.ActionEventSource follow_action;
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
        public class ExporterImport
        {
            [DllImport("exporter", CallingConvention = CallingConvention.StdCall, ExactSpelling = false, EntryPoint = "run_exporter")]
            private static extern int run_exporter(bool use_hidden, int use_max_triangles,
    bool recalc, int export_mode, int export_format);

            public int run_exporter_run(bool use_hidden, int use_max_triangles,
    bool recalc, int export_mode, int export_format)
            {
                return run_exporter(use_hidden, use_max_triangles, recalc, export_mode, export_format);
            }
        }
        /// <summary>
        /// Загрузка моей внешней C++ библиотеки, которая будет выполнять действия по геометрическому преобразованию объектов
        /// </summary>
        /// <param name="use_hidden"></param>
        /// <param name="use_max_triangles"></param>
        /// <param name="recalc"></param>
        /// <param name="export_mode"></param>
        /// <param name="export_format"></param>
        
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
                int wait = new ExporterImport().run_exporter_run(use_hidded_objects, max_triangles_count, use_recalc_coordinates, export_mode, export_format);
                int uu = 0;
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
