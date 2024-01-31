package main

// #cgo CFLAGS: -I. -I./lib/libch9326_x64/
// #cgo LDFLAGS: -L. -L./lib/libch9326_x64/ -lch9326
// #include "ch9326.h"
// #include "opm.h"
// #include "opm.c"
import "C"
import (
	"fmt"
	"log"
	"unsafe"

	"github.com/gotk3/gotk3/glib"
	"github.com/gotk3/gotk3/gtk"
)

func main() {
	devs := [1 << 8]C.struct_device{}
	r := C.open_opm_devices((**C.struct_device)(unsafe.Pointer(&devs[0])))
	fmt.Println(r)
	fmt.Println("Hello, World!")

	// Initialize GTK without parsing any command line arguments.
	gtk.Init(nil)

	// Create a new toplevel window, set its title, and connect it to the
	// "destroy" signal to exit the GTK main loop when it is destroyed.
	win, err := gtk.WindowNew(gtk.WINDOW_TOPLEVEL)
	if err != nil {
		log.Fatal("Unable to create window:", err)
	}
	win.SetTitle("Simple Example")
	win.Connect("destroy", func() {
		gtk.MainQuit()
	})

	grid, err := gtk.GridNew()
	if err != nil {
		log.Fatal("Unable to create grid:", err)
	}
	grid.SetColumnHomogeneous(false)
	grid.SetRowHomogeneous(true)
	win.Add(grid)

	grid.SetHExpand(true)
	grid.SetHAlign(gtk.ALIGN_FILL)

	// Create a list store for the list view
	listStore, err := gtk.ListStoreNew(glib.TYPE_STRING)
	if err != nil {
		log.Fatal("Unable to create list store:", err)
	}

	items := []string{}
	for i := 0; i < int(r); i++ {
		itemName := C.GoString((*C.char)(unsafe.Pointer(devs[i].manufacturer)))
		items = append(items, itemName)
	}
	for _, item := range items {
		iter := listStore.Append()
		err := listStore.Set(iter, []int{0}, []interface{}{item})
		if err != nil {
			log.Fatal("Unable to set list store value:", err)
		}
	}

	// Create a list view
	listView, err := gtk.TreeViewNewWithModel(listStore)
	if err != nil {
		log.Fatal("Unable to create list view:", err)
	}

	// Create a column for the list view
	renderer, err := gtk.CellRendererTextNew()
	if err != nil {
		log.Fatal("Unable to create cell renderer:", err)
	}
	column, err := gtk.TreeViewColumnNewWithAttribute("Device List", renderer, "text", 0)
	if err != nil {
		log.Fatal("Unable to create tree view column:", err)
	}
	listView.AppendColumn(column)
	grid.Attach(listView, 0, 0, 1, 1)

	// Column 2
	label1, err := gtk.LabelNew("Label 1")
	if err != nil {
		log.Fatal("Unable to create label:", err)
	}
	entry1, err := gtk.EntryNew()
	if err != nil {
		log.Fatal("Unable to create entry:", err)
	}
	label2, err := gtk.LabelNew("Label 2")
	if err != nil {
		log.Fatal("Unable to create label:", err)
	}
	entry2, err := gtk.EntryNew()
	if err != nil {
		log.Fatal("Unable to create entry:", err)
	}
	grid2, err := gtk.GridNew()
	if err != nil {
		log.Fatal("Unable to create grid:", err)
	}
	grid2.SetColumnHomogeneous(true)
	grid2.SetRowHomogeneous(true)
	grid2.Attach(label1, 0, 0, 1, 1)
	grid2.Attach(entry1, 1, 0, 1, 1)
	grid2.Attach(label2, 0, 1, 1, 1)
	grid2.Attach(entry2, 1, 1, 1, 1)
	grid.Attach(grid2, 1, 0, 1, 1)

	// Column 3
	label3, err := gtk.LabelNew("Column 3")
	if err != nil {
		log.Fatal("Unable to create label:", err)
	}
	grid.Attach(label3, 2, 0, 2, 1) // Set width to 2 instead of 1
	// Set the default window size.
	win.SetDefaultSize(1200, 600)

	// Recursively show all widgets contained in this window.
	win.ShowAll()

	// Begin executing the GTK main loop.  This blocks until
	// gtk.MainQuit() is run.
	gtk.Main()
}
