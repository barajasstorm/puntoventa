//
//  main.cpp
//  PuntoVentas
//
//  Created by Juan Barajas on 2/26/17.
//  Copyright © 2017 Juan Barajas. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <iomanip>

// function prototypes
void adminmenu();
void create();
void save();
void load();
void drop();
void modify();
void show();
void consult();
double search(struct item search);

// structures
struct item
{
    std::string name;
    double purchaseprice;
    double sellingprice;
    int stock;
    int minstock;
    int inactive;
};

// global variables
std::vector<item> item;


// main
int main()
{
    load();
    for(int i = 0; i < item.size(); i++)
    {
        std::cout << item[i].name << std::endl;
        std::cout << item[i].purchaseprice << std::endl;
        std::cout << item[i].sellingprice << std::endl;
        std::cout << item[i].stock << std::endl;
        std::cout << item[i].minstock << std::endl;
        std::cout << item[i].inactive << std::endl;
    }
    
    int ans;
    do
    {
        std::cout << "Menu Principal" << std::endl;
        std::cout << "1. Administrador" << std::endl;
        std::cout << "2. Ventas" << std::endl;
        std::cout << "3. Salir" << std::endl;
        std::cout << "Opcion: ";
        std::cin >> ans;
        std::cout << std::endl;
        
        switch(ans)
        {
            case 1: adminmenu();
                break;
            case 2:
                break;
            case 3:
                break;
            default:
                break;
        }
        
    }while(ans!=3);
    save();
    return 0;
}

// admin menu
void adminmenu()
{
    int ans;
    do
    {
        std::cout << "\nMenu de Administrador: \n";
        std::cout << "1. Altas\n";
        std::cout << "2. Bajas\n";
        std::cout << "3. Modificaciones\n";
        std::cout << "4. Mostrar Inventario\n";
        std::cout << "5. Consultar\n";
        std::cout << "6. Salir\n";
        std::cout << "Opcion: ";
        std::cin >> ans;
        std::cout << std::endl;
        switch(ans)
        {
            case 1: create();
                break;
            case 2: drop();
                break;
            case 3: modify();
                break;
            case 4: show();
                break;
            case 5: consult();
                break;
            case 6:
                break;
            default:
                std::cout << "La opcion seleccionada no es valida." << std::endl;
                std::cin.clear();
                break;
        }
    }while(ans!=6);
}

// create items
void create()
{
    int ans, ans2, ans3 = 0;
    double found;
    struct item articulo;
    do
    {
        std::cin.ignore();
        std::cout << "\n Ingresar nombre de producto: ";
        getline(std::cin, articulo.name);
        found = search(articulo);
        if(found == 0.1)
        {
            std::cout << "\nIngrese el precio de compra: ";
            std::cin >> articulo.purchaseprice;
            do{
                std::cout << "\nIngrese el precio de venta: ";
                std::cin >> articulo.sellingprice;
                if(articulo.sellingprice <= articulo.purchaseprice)
                {
                    std::cout << "El precio de venta debe ser mayor al precio de compra. Intentar de nuevo? (1 = Si, 2 = No): ";
                    std::cin >> ans3;
                }
                else
                {
                    ans3 = 2;
                }
                
            }while(ans3!=2);
            std::cout << "\nIngrese la cantidad en existencias: ";
            std::cin >> articulo.stock;
            std::cout << "\nIngrese el stock minimo requerido: ";
            std::cin >> articulo.minstock;
            articulo.inactive = 0;
            item.push_back(articulo);
            std::cout << "\nProducto agregado exitosamente. Agregar otro? (1 = SI, 2 = NO): ";
        }
        if(found < 0)
        {
            std::cout << "\nEl articulo '" << articulo.name << "' ya existe pero esta desactivado. Desea activarlo? (1 = SI, 2 = NO): ";
            std::cin >> ans2;
            if (ans2 == 1)
            {
                item[-found].inactive=0;
                std::cout << "\nProducto " << articulo.name << " activado exitosamente. Desea agregar otro producto? (1 = SI, 2 = NO): ";
            }
            else
            {
                std::cout << "\nDesea agregar otro producto? (1 = SI, 2 = NO): ";
            }
        }
        if(found == 0 || found > 0.1) std::cout << "\nEl producto ya existe. Desea agregar otro producto? (1 = SI, 2 = NO): ";
        std::cin >> ans;
    }while(ans!=2);
}

// delete items
void drop()
{
    int ans, ans2, ans3;
    double found;
    struct item articulo;
    std::cin.ignore();
    do
    {
        std::cout << "\nQue producto desea dar de baja? ";
        getline(std::cin, articulo.name);
        found = search(articulo);
        if(found == 0.1)
        {
            std::cout << "\nEl producto no existe.";
        }
        if(found < 0)
        {
            std::cout << "\nEl producto se encuentra desactivado. Desea borrarlo permanentemente? (1 = SI, 2 = NO): ";
            std::cin >> ans2;
            if (ans2 == 1){
                item.erase (item.begin()+(-found--));
                std::cout << "\nBorrado exitosamente" << std::endl;
            }
        }
        if(found == 0 || found > 0.1)
        {
            std::cout << "\nDesea borrar '" << articulo.name << "' permanentemente o solamente desactivarlo? (1 = Borrarlo, 2 = Desactivarlo, 3 = Cancelar): ";
            std::cin >> ans3;
            if (ans3 == 1){
                item.erase (item.begin()+(found--));
                std::cout << "\nBorrado exitosamente" << std::endl;
            } else {
                if (ans3 == 2){
                    item[found].inactive=1;
                    std::cout << "\nDesactivado exitosamente" << std::endl;
                }
            }
        }
        std::cout << "\nDesea dar de baja otro producto? (1 = SI, 2 = NO): ";
        std::cin >> ans;
        std::cin.ignore();
    }while(ans!=2);
}

// modify items
void modify()
{
    struct item articulo;
    double found;
    int ans, ans2, ans3, ans4;
    do
    {
        std::cin.ignore();
        std::cout << "Que producto desea modificar? ";
        getline(std::cin, articulo.name);
        found = search(articulo);
        if (found == 0 || found > 0.1)
        {
            std::cout << std::endl;
            std::cout << "Producto" << std::setw(17) << "$Compra  " << std::setw(10) << "$Venta  " << std::setw(17) << "Existencias  " << std::setw(10) << "Stock Min" << std::endl;
            std::cout << "______________________________________________________________" << std::endl;
            std::cout << item[found].name << std::setw(15) << item[found].purchaseprice << std::setw(10) << item[found].sellingprice << std::setw(13) << item[found].stock << std::setw(12) << item[found].minstock << std::endl;
            std::cout << std::endl;
            std::cout << "1. Nombre" << std::endl;
            std::cout << "2. $Compra" << std::endl;
            std::cout << "3. $Venta" << std::endl;
            std::cout << "4. Existencias" << std::endl;
            std::cout << "5. Stock" << std::endl;
            std::cout << std::endl;
            std::cout << "Ingrese el numero correspondiente para modificar el campo deseado: ";
            std::cin >> ans3;
            std::cout << std::endl;
            switch(ans3){
                case 1:
                    std::cin.ignore();
                    std::cout << "Ingresar el nuevo nombre para el producto '" << item[found].name << "': ";
                    getline(std::cin, articulo.name);
                    std::cout << std::endl;
                    std::cout << "Confirmar el cambio de '" << item[found].name << "' a '" << articulo.name << "' (1 = Confirmar, 2 = Cancelar): ";
                    std::cin >> ans4;
                    std::cout << std::endl;
                    if (ans4==1){
                        item[found].name = articulo.name;
                        std::cout << "Cambio exitoso." << std::endl;
                        std::cout << std::endl;
                    }
                    else if (ans4==2){
                        std::cout << "Cancelado" << std::endl;
                        std::cout << std::endl;
                    }
                    break;
                case 2:
                    std::cout << "Ingresar el nuevo precio de $Compra para el producto '" << item[found].name << "': ";
                    std::cin >> articulo.purchaseprice;
                    std::cout << std::endl;
                    std::cout << "Confirmar el cambio de '" << item[found].purchaseprice << "' a '" << articulo.purchaseprice << "' (1 = Confirmar, 2 = Cancelar): ";
                    std::cin >> ans4;
                    std::cout << std::endl;
                    if (ans4==1){
                        item[found].purchaseprice = articulo.purchaseprice;
                        std::cout << "Cambio exitoso." << std::endl;
                        std::cout << std::endl;
                    }
                    else if (ans4==2){
                        std::cout << "Cancelado" << std::endl;
                        std::cout << std::endl;
                    }
                    break;
                case 3:
                    std::cout << "Ingresar el nuevo precio de $Venta para el producto '" << item[found].name << "': ";
                    std::cin >> articulo.sellingprice;
                    std::cout << std::endl;
                    std::cout << "Confirmar el cambio de '" << item[found].sellingprice << "' a '" << articulo.sellingprice << "' (1 = Confirmar, 2 = Cancelar): ";
                    std::cin >> ans4;
                    std::cout << std::endl;
                    if (ans4==1){
                        item[found].sellingprice = articulo.sellingprice;
                        std::cout << "Cambio exitoso." << std::endl;
                        std::cout << std::endl;
                    }
                    else if (ans4==2){
                        std::cout << "Cancelado" << std::endl;
                        std::cout << std::endl;
                    }
                    break;
                case 4:
                    std::cout << "Ingresar el nuevo numero de Existencias para el producto '" << item[found].name << "': ";
                    std::cin >> articulo.stock;
                    std::cout << std::endl;
                    std::cout << "Confirmar el cambio de '" << item[found].stock << "' a '" << articulo.stock << "' (1 = Confirmar, 2 = Cancelar): ";
                    std::cin >> ans4;
                    std::cout << std::endl;
                    if (ans4==1){
                        item[found].stock = articulo.stock;
                        std::cout << "Cambio exitoso." << std::endl;
                        std::cout << std::endl;
                    }
                    else if (ans4==2){
                        std::cout << "Cancelado" << std::endl;
                        std::cout << std::endl;
                    }
                    break;
                case 5:
                    std::cout << "Ingresar el nuevo numero de Stock Minimo para el producto '" << item[found].name << "': ";
                    std::cin >> articulo.minstock;
                    std::cout << std::endl;
                    std::cout << "Confirmar el cambio de '" << item[found].minstock << "' a '" << articulo.minstock << "' (1 = Confirmar, 2 = Cancelar): ";
                    std::cin >> ans4;
                    std::cout << std::endl;
                    if (ans4==1){
                        item[found].minstock = articulo.minstock;
                        std::cout << "Cambio exitoso." << std::endl;
                        std::cout << std::endl;
                    }
                    else if (ans4==2){
                        std::cout << "Cancelado" << std::endl;
                        std::cout << std::endl;
                    }
                    break;
                default:
                    std::cout << "La opcion no es validad. ";
                    break;
            }
            if (found < 0)
            {
                std::cout << "El producto se encuentra desactivado. Aun asi desea modificarlo? (1 = SI, 2 = NO): ";
                std::cin >> ans2;
                if (ans2==1)
                {
                    std::cout << std::endl;
                    std::cout << "         El producto se encuentra inactivo!     " << std::endl << std::endl;
                    std::cout << "Producto" << std::setw(17) << "$Compra" << std::setw(10) << "$Venta" << std::setw(17) << "Existencias" << std::setw(10) << "Stock" << std::endl;
                    std::cout << "_________________________________________________________________" << std::endl;
                    std::cout << item[-found].name << std::setw(15) << item[-found].purchaseprice << std::setw(10) << item[-found].sellingprice << std::setw(13) << item[-found].stock << std::setw(12) << item[-found].minstock << std::endl;
                    std::cout << std::endl;
                    std::cout << "1. Nombre" << std::endl;
                    std::cout << "2. $Compra" << std::endl;
                    std::cout << "3. $Venta" << std::endl;
                    std::cout << "4. Existencias" << std::endl;
                    std::cout << "5. Stock" << std::endl;
                    std::cout << "6. Activarlo" << std::endl;
                    std::cout << "Ingrese el numero correspondiente para modificar el campo deseado: ";
                    std::cin >> ans3;
                    switch(ans3)
                    {
                        case 1:
                            std::cin.ignore();
                            std::cout << "Ingresar el nuevo nombre para el producto '" << item[-found].name << "': ";
                            getline(std::cin, articulo.name);
                            std::cout << "Confirmar el cambio de '" << item[-found].name << "' a '" << articulo.name << "' (1 = Confirmar, 2 = Cancelar): ";
                            std::cin >> ans4;
                            if (ans4==1)
                            {
                                item[-found].name = articulo.name;
                                std::cout << "Cambio exitoso." << std::endl;
                                std::cout << std::endl;
                            }
                            else if (ans4==2)
                            {
                                std::cout << "Cancelado" << std::endl;
                                std::cout << std::endl;
                            }
                            break;
                        case 2:
                            std::cout << "Ingresar el nuevo precio de $Compra para el producto '" << item[-found].name << "': ";
                            std::cin >> articulo.purchaseprice;
                            std::cout << "Confirmar el cambio de '" << item[-found].purchaseprice << "' a '" << articulo.purchaseprice << "' (1 = Confirmar, 2 = Cancelar): ";
                            std::cin >> ans4;
                            if (ans4==1)
                            {
                                item[-found].purchaseprice = articulo.purchaseprice;
                                std::cout << "Cambio exitoso." << std::endl;
                                std::cout << std::endl;
                            }
                            else if (ans4==2)
                            {
                                std::cout << "Cancelado" << std::endl;
                                std::cout << std::endl;
                            }
                            break;
                        case 3:
                            std::cout << "Ingresar el nuevo precio de $Venta para el producto '" << item[-found].sellingprice << "': ";
                            std::cin >> articulo.sellingprice;
                            std::cout << "Confirmar el cambio de '" << item[-found].sellingprice << "' a '" << articulo.sellingprice << "' (1 = Confirmar, 2 = Cancelar): ";
                            std::cin >> ans4;
                            if (ans4==1)
                            {
                                item[-found].sellingprice = articulo.sellingprice;
                                std::cout << "Cambio exitoso." << std::endl;
                                std::cout << std::endl;
                            }
                            else if (ans4==2)
                            {
                                std::cout << "Cancelado" << std::endl;
                                std::cout << std::endl;
                            }
                            break;
                        case 4:
                            std::cout << "Ingresar el nuevo numero de Existencias para el producto '" << item[-found].stock << "': ";
                            std::cin >> articulo.stock;
                            std::cout << "Confirmar el cambio de '" << item[-found].stock << "' a '" << articulo.stock << "' (1 = Confirmar, 2 = Cancelar): ";
                            std::cin >> ans4;
                            if (ans4==1)
                            {
                                item[-found].stock = articulo.stock;
                                std::cout << "Cambio exitoso." << std::endl;
                                std::cout << std::endl;
                            }
                            else if (ans4==2)
                            {
                                std::cout << "Cancelado" << std::endl;
                                std::cout << std::endl;
                            }
                            break;
                        case 5:
                            std::cout << "Ingresar el nuevo numero de Stock Minimo para el producto '" << item[-found].minstock << "': ";
                            std::cin >> articulo.minstock;
                            std::cout << "Confirmar el cambio de '" << item[-found].minstock << "' a '" << articulo.minstock << "' (1 = Confirmar, 2 = Cancelar): ";
                            std::cin >> ans4;
                            if (ans4==1)
                            {
                                item[-found].minstock = articulo.minstock;
                                std::cout << "Cambio exitoso." << std::endl;
                                std::cout << std::endl;
                            }
                            else if (ans4==2)
                            {
                                std::cout << "Cancelado" << std::endl;
                                std::cout << std::endl;
                            }
                            break;
                        case 6:
                            std::cout << "Dease activar el producto '" << item[-found].inactive << "'? (1 = Confirmar, 2 = Cancelar): ";
                            std::cin >> ans4;
                            if (ans4==1)
                            {
                                item[-found].inactive = 0;
                                std::cout << "Cambio exitoso." << std::endl;
                                std::cout << std::endl;
                            }
                            else if (ans4==2)
                            {
                                std::cout << "Cancelado" << std::endl;
                                std::cout << std::endl;
                            }
                            break;
                        default:
                            std::cout << "La opcion no es validad. ";
                            break;
                    }
                }
            }
        }
        if (found == 0.1) std::cout << "El producto no existe. " << std::endl;
        std::cout << "Desea modificar otro producto? (1 = SI, 2 = NO): ";
        std::cin >> ans;
        std::cout << std::endl;
    }while(ans!=2);
}

// show all inventory items
void show()
{
    std::cout << std::endl;
    std::cout << "Producto" << std::setw(15) << "$Compra" << std::setw(10) << "$Venta" << std::setw(17) << "Existencias" << std::setw(16) << "Stock Min" << std::endl;
    std::cout << "________________________________________________________________________" << std::endl;
    for (int i = 0; i < item.size(); i++){
        if(item[i].inactive == 0) std::cout << item[i].name << std::setw(15) << item[i].purchaseprice << std::setw(10) << item[i].sellingprice << std::setw(15) << item[i].stock << std::setw(15) << item[i].minstock;
        if(item[i].stock < item[i].minstock){
            std::cout << " *" << std::endl;
        } else {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

// show one inventory item
void consult(){
    struct item articulo;
    int ans = 0;
    double found;
    do{
        std::cin.ignore();
        std::cout << "Que producto deseas consultar? ";
        getline(std::cin, articulo.name);
        found = search(articulo);
        if (found == 0.1){
            std::cout << "El producto no existe. ";
        }
        if (found < 0){
            std::cout << std::endl;
            std::cout << "         El producto se encuentra inactivo!     " << std::endl << std::endl;
            std::cout << "Producto" << std::setw(15) << "$Compra" << std::setw(10) << "$Venta" << std::setw(15) << "Existencias" << std::setw(9) << "Stock" << std::endl;
            std::cout << "________________________________________________________________________" << std::endl;
            std::cout << item[-found].name << std::setw(15) << item[-found].purchaseprice << std::setw(10) << item[-found].sellingprice << std::setw(13) << item[-found].stock << std::setw(12) << item[-found].minstock << std::endl;
            std::cout << std::endl;
        }
        if (found == 0 || found > 0.1){
            std::cout << std::endl;
            std::cout << "Producto" << std::setw(15) << "$Compra" << std::setw(10) << "$Venta" << std::setw(15) << "Existencias" << std::setw(9) << "Stock" << std::endl;
            std::cout << "________________________________________________________________________" << std::endl;            std::cout << item[found].name << std::setw(15) << item[found].purchaseprice << std::setw(10) << item[found].sellingprice << std::setw(13) << item[found].stock << std::setw(12) << item[found].minstock << std::endl;
            std::cout << std::endl;        }
        std::cout << "Deseas consultar otro producto? (1 = SI, 2 = NO): ";
        std::cin >> ans;
    }while(ans!=2);
}



// search for item in inventory and return 0.1 for not found, x < 0 for inactive, x == 0 || x > 0.1 for active
double search(struct item search){
    double found = 0.1;
    for(int x = 0; x < item.size(); x++)
    {
        std::cout << item[x].name << std::endl;
        if (search.name == item[x].name)
        {
            if (item[x].inactive==0)
            {
                found = x;
                break;
            }
            else
            {
                found = -x;
                break;
            }
        }
        else
        {
            found = 0.1;
        }
    }
    return found;
}

// save items to file
void save()
{
    std::ofstream outfile;
    outfile.open("inventory.bin", std::ios::binary);
    for(int i = 0; i < item.size(); i++)
    {
        outfile.write(reinterpret_cast<char *> (&item[i].name),sizeof(item[i].name));
        outfile.write(reinterpret_cast<char *> (&item[i].purchaseprice),sizeof(item[i].purchaseprice));
        outfile.write(reinterpret_cast<char *> (&item[i].sellingprice),sizeof(item[i].sellingprice));
        outfile.write(reinterpret_cast<char *> (&item[i].stock),sizeof(item[i].stock));
        outfile.write(reinterpret_cast<char *> (&item[i].minstock),sizeof(item[i].minstock));
        outfile.write(reinterpret_cast<char *> (&item[i].inactive),sizeof(item[i].inactive));
    }
    outfile.close();
}

// load items from file
void load()
{
    struct item articulo;
    std::ifstream infile;
    std::ofstream outfile;
    infile.open("inventory.bin", std::ios::binary);
    if(infile.fail())
    {
        infile.close();
        outfile.open("inventory.bin", std::ios::binary);
        outfile.close();
        std::cout << "Archivo inventory.bin creado. Favor de reiniciar programa." << std::endl;
        exit (EXIT_FAILURE);
    }
    if(!infile.fail())
    {
        std::cout << "Archivo inventory.bin cargado exitosamente." << std::endl << std::endl;
    }
    if (infile.peek() != std::ifstream::traits_type::eof())
    {
        while(!infile.eof())
        {
            infile.read(reinterpret_cast<char *> (&articulo.name),sizeof(articulo.name));
            infile.read(reinterpret_cast<char *> (&articulo.purchaseprice),sizeof(articulo.purchaseprice));
            infile.read(reinterpret_cast<char *> (&articulo.sellingprice),sizeof(articulo.sellingprice));
            infile.read(reinterpret_cast<char *> (&articulo.stock),sizeof(articulo.stock));
            infile.read(reinterpret_cast<char *> (&articulo.minstock),sizeof(articulo.minstock));
            infile.read(reinterpret_cast<char *> (&articulo.inactive),sizeof(articulo.inactive));
            item.push_back(articulo);
        }
        item.pop_back();
    }
    infile.close();
}

















