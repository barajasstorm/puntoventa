//
//  main.cpp
//  PuntoVentas
//
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
void newuser();
void edituser();
void usersAdminMenu();
void showusers();
void sales();
void salesuser(int userid);
void salesday(int userid);
double search(struct item search);
bool searchuser(struct user name);
void viewsales(int userid);
void salesreport();
void allsales();
void viewsalesperemployee();
void settoRand(); //hidden admin option 9, set all stock items to rand between 0 and 30
void outputcsv(); //hideen admin option 10, save item list to CSV file
void sortreport(std::vector<struct ssort> list, int type);
void sort(std::vector<struct item> vsort, int type);

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
struct user
{
  std::string name;
  std::string password;
};
struct transaction
{
  int ticketID; //ticket foreign key
  int salespersonID; //user foreign key
  std::string description;
  double quantity;
  double price;
  double total;
};
struct ticket
{
  int corteID; //corte foreign key
  int numTransactions;
  double total;
};
struct corte
{
  int dayID;//day foreign key
  int numTickets;
  int salespersonID; //user foreign key
  double total;
};
struct day
{
  int day;
  int month;
  int year;
};
struct ssort
{
  int day;
  int month;
  int year;
  int corteID;
  int ticketID;
  int quantity;
  double total;
  std::string description;
  std::string name;
};

// global variables
std::vector<struct item> item;
std::vector<struct item> vsort;
std::vector<struct user> user;
std::vector<struct day> days;
std::vector<struct transaction> transaction;
std::vector<struct corte> corte;
std::vector<struct ticket> ticket;
std::vector<struct ssort> ssort;

// main
int main()
{
  load();
  int ans, password;
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
      case 1:
      {
        std::cout << "Ingresar contraseña de administrador (1234): ";
        std::cin >> password;
        if (password==1234){
          adminmenu();
        }
        else {
          std::cout << "\nLa contraseña es incorrecta. Por favor intente de nuevo.\n\n";
        }
      }
        break;
      case 2: sales();
        break;
      case 3:
        break;
      default:
        break;
    }
    
  }while(ans!=3);
  save();
  show();
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
    std::cout << "6. Usuarios\n";
    std::cout << "7. Ventas Reporte\n";
    std::cout << "8. Salir\n";
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
      case 6: usersAdminMenu();
        break;
      case 7: salesreport();
        break;
      case 8:
        break;
      case 9: settoRand();
        break;
      case 10: outputcsv();
        break;
      default:
        std::cout << "La opcion seleccionada no es valida." << std::endl;
        std::cin.clear();
        break;
    }
  }while(ans!=8);
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
    std::cout << "\nIngresar nombre de producto: ";
    getline(std::cin, articulo.name);
    articulo.name[0] = toupper(articulo.name[0]);
    for(int x = 1; x < articulo.name.size(); x++) articulo.name[x] = tolower(articulo.name[x]);
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
    articulo.name[0] = toupper(articulo.name[0]);
    for(int x = 1; x < articulo.name.size(); x++) articulo.name[x] = tolower(articulo.name[x]);
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
  double found, found2;
  int ans, ans2, ans3, ans4, ans5 = 0;
  do
  {
    std::cin.ignore();
    std::cout << "Que producto desea modificar? ";
    getline(std::cin, articulo.name);
    articulo.name[0] = toupper(articulo.name[0]);
    for(int x = 1; x < articulo.name.size(); x++) articulo.name[x] = tolower(articulo.name[x]);
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
          do
          {
            std::cin.ignore();
            std::cout << "Ingresar el nuevo nombre para el producto '" << item[found].name << "': ";
            getline(std::cin, articulo.name);
            articulo.name[0] = toupper(articulo.name[0]);
            for(int x = 1; x < articulo.name.size(); x++) articulo.name[x] = tolower(articulo.name[x]);
            found2 = search(articulo);
            if (found == 0 || found > 0.1 || found < 0) std::cout << "El nombre ya esta en uso" << std::endl;
            if (found == 0.1)
            {
              std::cout << std::endl;
              std::cout << "Confirmar el cambio de '" << item[found].name << "' a '" << articulo.name << "' (1 = Confirmar, 2 = Cancelar): ";
              std::cin >> ans4;
              std::cout << std::endl;
              if (ans4==1){
                item[found].name = articulo.name;
                std::cout << "Cambio exitoso." << std::endl;
                std::cout << std::endl;
                ans5 = 2;
              }
              else if (ans4==2){
                std::cout << "Cancelado. Desea intentar otro nombre? (1 = SI, 2 =  NO): ";
                std::cin >> ans5;
                std::cout << std::endl;
              }
            }
          }while(ans5 == 1);
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
              do
              {
                std::cin.ignore();
                std::cout << "Ingresar el nuevo nombre para el producto '" << item[-found].name << "': ";
                getline(std::cin, articulo.name);
                articulo.name[0] = toupper(articulo.name[0]);
                for(int x = 1; x < articulo.name.size(); x++) articulo.name[x] = tolower(articulo.name[x]);
                found2 = search(articulo);
                if (found == 0 || found > 0.1 || found < 0) std::cout << "El nombre ya esta en uso" << std::endl;
                if (found == 0.1)
                {
                  std::cout << std::endl;
                  std::cout << "Confirmar el cambio de '" << item[-found].name << "' a '" << articulo.name << "' (1 = Confirmar, 2 = Cancelar): ";
                  std::cin >> ans4;
                  std::cout << std::endl;
                  if (ans4==1){
                    item[-found].name = articulo.name;
                    std::cout << "Cambio exitoso." << std::endl;
                    std::cout << std::endl;
                    ans5 = 2;
                  }
                  else if (ans4==2){
                    std::cout << "Cancelado. Desea intentar otro nombre? (1 = SI, 2 =  NO): ";
                    std::cin >> ans5;
                    std::cout << std::endl;
                  }
                }
              }while(ans5 == 1);
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
  vsort = item;
  int ans;
  do
  {
    std::cout << std::endl;
    std::cout << std::setw(15) << std::left << "Producto" << std::setw(15) << "\t $Compra" << std::setw(10) << "\t$Venta" << "Existencias" << std::setw(15) << std::right << "Stock Min" << std::endl;
    std::cout << "________________________________________________________________________" << std::endl;
    for (int i = 0; i < item.size(); i++){
      if(vsort[i].inactive == 0) std::cout << std::setw(20) << std::left << vsort[i].name << std::setw(15) << vsort[i].purchaseprice << std::setw(10) << vsort[i].sellingprice << std::setw(15) << vsort[i].stock << std::setw(3) << vsort[i].minstock;
      if(vsort[i].stock <= vsort[i].minstock)
      {
        std::cout << " *" << std::endl;
      }
      else
      {
        std::cout << std::endl;
      }
    }
    std::cout << std::endl;
    
    std::cout << "Mostrar listado en orden: " << std::endl;
    std::cout << "1. Nombre de Producto A - Z" << std::endl;
    std::cout << "2. Nombre de Producto Z - A" << std::endl;
    std::cout << "3. Existencias Menor a Mayor" << std::endl;
    std::cout << "4. Salir" << std::endl;
    std::cout << "Opcion: ";
    std::cin >> ans;
    if(ans!=4) sort(item,ans);
  }while(ans!=4);
}

// show one inventory item
void consult()
{
  struct item articulo;
  int ans = 0;
  double found;
  do{
    std::cin.ignore();
    std::cout << "Que producto deseas consultar? ";
    getline(std::cin, articulo.name);
    articulo.name[0] = toupper(articulo.name[0]);
    for(int x = 1; x < articulo.name.size(); x++) articulo.name[x] = tolower(articulo.name[x]);
    found = search(articulo);
    if (found == 0.1)
    {
      std::cout << "El producto no existe. ";
    }
    if (found < 0)
    {
      std::cout << std::endl;
      std::cout << "         El producto se encuentra inactivo!     " << std::endl << std::endl;
      std::cout << std::setw(15) << std::left << "Producto" << std::setw(17) << "\t $Compra" << std::setw(10) << "$Venta" << std::setw(12) << "Stock" << std::setw(13) << "Stock Min" << std::endl;
      std::cout << "________________________________________________________________________" << std::endl;            std::cout << std::setw(20) << std::left << item[-found].name << std::setw(15) << item[-found].purchaseprice << std::setw(10) << item[-found].sellingprice << std::setw(15) << item[-found].stock << std::setw(3) << item[-found].minstock;
      if(item[-found].stock <= item[-found].minstock){
        std::cout << " *" << std::endl;
      } else {
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }
    if (found == 0 || found > 0.1)
    {
      std::cout << std::endl;
      std::cout << std::setw(15) << std::left << "Producto" << std::setw(17) << "\t $Compra" << std::setw(10) << "$Venta" << std::setw(12) << "Stock" << std::setw(13) << "Stock Min" << std::endl;
      std::cout << "________________________________________________________________________" << std::endl;            std::cout << std::setw(20) << std::left << item[found].name << std::setw(15) << item[found].purchaseprice << std::setw(10) << item[found].sellingprice << std::setw(15) << item[found].stock << std::setw(3) << item[found].minstock;
      if(item[found].stock <= item[found].minstock){
        std::cout << " *" << std::endl;
      } else {
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }
    std::cout << "Deseas consultar otro producto? (1 = SI, 2 = NO): ";
    std::cin >> ans;
  }while(ans!=2);
}

void usersAdminMenu()
{
  int ans;
  do
  {
    std::cout << "\nMenu de Usuarios\n";
    std::cout << "1. Crear Usuario Nuevo\n";
    std::cout << "2. Modificar Usuario\n";
    std::cout << "3. Mostrar Todos\n";
    std::cout << "4. Salir\n";
    std::cout << "Opcion: ";
    std::cin >> ans;
    std::cout << std::endl;
    
    switch(ans)
    {
      case 1:
        if (user.size() < 5)
        {
          newuser();
        }
        else
        {
          std::cout << "Solo se permite un maximo de 5 usuarios.\n";
        }
        break;
      case 2:
        edituser();
        break;
      case 3:
        showusers();
        break;
      case 4:
        break;
      default:
        std::cout << "Opcion seleccionada no es valida." << std::endl;
        break;
    }
  }while(ans!=4);
}

// create new user to user vector
void newuser()
{
  int ans = 0;
  struct user usuario;
  bool found;
  do
  {
    std::cin.ignore();
    std::cout << "Creando usuario..." << std::endl;
    std::cout << "Ingresar nombre de usuario: ";
    getline(std::cin, usuario.name);
    usuario.name[0] = toupper(usuario.name[0]);
    for(int x = 1; x < usuario.name.size(); x++) usuario.name[x] = tolower(usuario.name[x]);
    found = searchuser(usuario);
    if (found==false)
    {
      std::cout << std::endl;
      std::cout << "Ingresar contrasena: ";
      getline(std::cin, usuario.password);
      std::cout << std::endl << "Usuario '" << usuario.name << "' creado exitosamente.\n" << std::endl;
      std::cout << "Desea crear otro usuario? (1 = SI, 2 = NO): ";
      std::cin >> ans;
      std::cout << std::endl;
      user.push_back(usuario);
      if (ans==1 && user.size()==5) std::cout << "Solo se permite un maximo de 5 usuarios.\n";
    }
    else
    {
      std::cout << "\nEl usario ya existe. Desea intentar de nuevo? (1 = SI, 2 = NO): ";
      std::cin >> ans;
      std::cout << std::endl;
    }
  }while(ans!=2 && user.size() < 5);
  
}

// search for a user in user list and return true or false
bool searchuser(struct user name)
{
  int i = 0;
  double found = 0;
  while( i < user.size())
  {
    if (name.name == user[i].name)
    {
      found = true;
    }
    else
    {
      found = false;
    }
    i++;
  }
  return found;
}

// edit user
void edituser(){
  int ans = 0, ans2 = 0;
  int userid, modtype;
  bool found;
  struct user temporary;
  std::cin.ignore();
  do{
    std::cout << "Escribe # de Usuario" << std::endl;
    std::cout << "    A Modificar       " << std::endl;
    std::cout << "___________________" << std::endl;
    for (int i = 0; i < user.size(); i++){
      std::cout << i+1;
      std::cout << ":" << "\t\t" << user[i].name << std::endl;
    }
    std::cout << std::endl << "Usuario: ";
    std::cin >> userid;
    std::cout << std::endl;
    std::cout << "Selecciona opcion a modificar:" << std::endl;
    std::cout << "1. Nombre" << std::endl;
    std::cout << "2. Contraseña" << std::endl;
    std::cout << "3. Remover Usuario" << std::endl;
    std::cout << "Opcion: ";
    std::cin >> modtype;
    if(modtype==1){
      std::cout << "\nIngresar nuevo nombre para el usuario '" << user[userid-1].name << "': ";
      std::cin.ignore();
      getline(std::cin, temporary.name);
      temporary.name[0] = toupper(temporary.name[0]);
      for(int x = 1; x < temporary.name.size(); x++) temporary.name[x] = tolower(temporary.name[x]);
      found = searchuser(temporary);
      if(found == true) std::cout << "El nombre ya esta en uso." << std::endl;
      else
      {
        user[userid-1].name = temporary.name;
        std::cout << "\nCambio exitoso!" << std::endl << std::endl;
      }
    }
    if(modtype==2){
      std::cout << "\nIngresar nueva contraseña para el usuario '" << user[userid-1].name << "': ";
      std::cin.ignore();
      getline(std::cin, temporary.password);
      user[userid-1].password = temporary.password;
      std::cout << "\nCambio exitoso!" << std::endl << std::endl;
    }
    if(modtype==3){
      std::cout << "\nEsta seguro que desea borrar el usuario '" << user[userid-1].name << "' permanentemente? (1 = SI, 2 = NO): " << std::endl;
      std::cin >> ans2;
      if(ans2==1){
        user.erase (user.begin()+(userid-1));
        std::cout << "\nUsuario fue dado de baja exitosamente!" << std::endl;}
      
    }
    std::cout << "Deseas modificar otro usuario? (1 = SI, 2 = NO): ";
    std::cin >> ans;
    std::cout << std::endl;
  }while(ans!=2);
}

// show all users
void showusers(){
  std::cout << "Usuarios de Ventas" << std::endl;
  std::cout << "___________________" << std::endl;
  for (int i = 0; i < user.size(); i++)
  {
    std::cout << i+1;
    std::cout << ":" << "\t\t" << user[i].name << std::endl;
  }
}

// sales menu
void sales()
{
  int userid, ans;
  struct user temporary;
  do
  {
    std::cout << "Seleccionar Usuario:" << std::endl;
    std::cout << "_____________________" << std::endl;
    for (int i = 0; i < user.size(); i++)
    {
      std::cout << i+1;
      std::cout << ":" << "\t\t" << user[i].name << std::endl;
    }
    std::cout << std::endl << "Usuario: ";
    std::cin >> userid;
    std::cin.ignore();
    std::cout << "Favor de ingresar contraseña: ";
    getline(std::cin, temporary.password);
    std::cout << std::endl;
    if (temporary.password == user[userid-1].password)
    {
      salesuser(userid-1);
      ans=2;
    }
    else
    {
      std::cout << "La contraseña es incorrecta. Desea intentar de nuevo (1 = SI, 2 = NO): ";
      std::cin >> ans;
      std::cout << std::endl;
    }
  }while(ans!=2);
}

// sales per user
void salesuser(int userid){
  int ans;
  std::cout << "Hola " << user[userid].name << "!" << std::endl;
  while(ans!=3)
  {
    std::cout << "\nSeleccionar opcion:" << std::endl;
    std::cout << "1. Iniciar Dia de Ventas" << std::endl;
    std::cout << "2. Visualizar Mis Ventas" << std::endl;
    std::cout << "3. Salir" << std::endl;
    std::cout << "Opecion: ";
    std::cin >> ans;
    std::cout << std::endl;
    switch(ans){
      case 1:
        salesday(userid);
        break;
      case 2:
        viewsales(userid);
        break;
      case 3:
        break;
      default:
        break;
    }
  }
}

// sales day
void salesday(int userid)
{
  int ans, ans2, ans3 = 0, transaction_count, corte_count, ticket_count, num_transactions, num_tickets, days_count, quantity = 0;
  struct item itemTmp;
  double ticket_total, corte_total, found;
  do
  { // day of sales
    struct day diaTmp;
    time_t t = time(0);
    struct tm * now = localtime( & t );
    diaTmp.year =now->tm_year + 1900;
    diaTmp.month = now->tm_mon + 1;
    diaTmp.day = now->tm_mday;
    days.push_back(diaTmp);
    days_count = 0;
    for(int i = 0; i < days.size(); i++) days_count++;
    std::cout << diaTmp.day << "/" << diaTmp.month << "/" << diaTmp.year << std::endl;
    do
    { //corte
      struct corte corteTmp;
      corte_count = 0;
      for(int i = 0; i < corte.size(); i++) corte_count++;
      std::cout << "Corte #" << corte_count << std::endl;
      corte_total = 0;
      num_tickets = 0;
      do
      { //ticket
        struct ticket ticketTmp;
        ticket_count = 0;
        for(int i = 0; i < ticket.size(); i++) ticket_count++;
        std::cout << "Ticket #" << ticket_count << std::endl;
        num_transactions = 0;
        ticket_total = 0;
        do
        { //transaction
          struct transaction transTmp;
          transaction_count = 0;
          for(int i = 0; i < transaction.size(); i++) transaction_count++;
          std::cout << "Transaccion #" << transaction_count << std::endl;
          std::cin.ignore();
          std::cout << "\nNombre de producto a vender: ";
          getline(std::cin, itemTmp.name);
          itemTmp.name[0] = toupper(itemTmp.name[0]);
          for(int y = 1; y < itemTmp.name.size(); y++) itemTmp.name[y] = tolower(itemTmp.name[y]);
          found = search(itemTmp);
          if (found==0 || found>0.1)
          {
            do
            {
              std::cout << "Cantidad a vender, tiene " << item[found].stock << " en existencias: ";
              std::cin >> quantity;
              std::cout << std::endl;
              if (quantity <= item[found].stock)
              {
                item[found].stock -= quantity;
                std::cout << "************************************************" << std::endl;
                std::cout << std::endl << "Ticket de Venta #" << ticket_count << "\t\t Fecha: " << diaTmp.day << "/" << diaTmp.month << "/" << diaTmp.year << std::endl;
                std::cout << std::endl << "Cant \tDescripcion      \tPrecio    \tImporte" << std::endl;
                std::cout << "_____________________________________________________" << std::endl;
                std::cout << quantity  << "\t\t" << item[found].name << "\t\t\t$ " << item[found].sellingprice << "\t\t$ " << quantity * item[found].sellingprice << std::endl;
                transTmp.ticketID = ticket_count;
                transTmp.salespersonID = userid;
                transTmp.description = item[found].name;
                transTmp.quantity = quantity;
                transTmp.price = item[found].sellingprice;
                transTmp.total = transTmp.quantity * transTmp.price;
                transaction.push_back(transTmp);
                num_transactions++;
                ticket_total += transTmp.total;
                ans3=2;
              }
              if (quantity > item[found].stock  && ans3!=2)
              {
                std::cout << "No se cuenta con suficiente cantidad en existencias. Desea ingresar otra cantidad? (1 = SI, 2 = NO): ";
                std::cin >> ans3;
              }
            }while(ans3!=2);
          }
          else
          {
            std::cout << "\nEl producto no existe o esta desactivado." << std::endl;
          }
          std::cout << "\n1. Vender Otro Producto";
          std::cout << "\n2. Cerrar Ticket";
          std::cout << "\nOpcion: ";
          std::cin >> ans;
          std::cout << std::endl;
        }while(ans!=2);
        ticketTmp.corteID = corte_count;
        ticketTmp.numTransactions = num_transactions;
        ticketTmp.total = ticket_total;
        ticket.push_back(ticketTmp);
        corte_total += ticketTmp.total;
        num_tickets++;
        std::cout << std::endl;
        std::cout << "*****************************************************" << std::endl;
        std::cout << "Vendedor: " << user[userid].name << std::endl;
        std::cout << "Ticket de Venta #" << ticket_count << "\t\t Fecha: " << diaTmp.day << "/" << diaTmp.month << "/" << diaTmp.year << std::endl;
        std::cout << std::endl << "Cant \tDescripcion      \tPrecio      \tImporte" << std::endl;
        std::cout << "______________________________________________________" << std::endl;
        
        for(int i = 0; i < transaction.size(); i++)
        {
          if(transaction[i].ticketID == ticket_count)
          {
            std::cout << transaction[i].quantity  << "\t\t" << std::setw(11) << std::internal << transaction[i].description << std::left << "\t\t$ " << transaction[i].price << "\t\t$ " << transaction[i].total << std::endl;
          }
        }
        std::cout << std::endl;
        std::cout << "                                  Total: $" << ticketTmp.total << std::endl;
        std::cout << "*****************************************************" << std::endl;
        std::cout << std::endl;
        std::cout << "\n1. Iniciar Nueva Venta";
        std::cout << "\n2. Realizar Corte";
        std::cout << "\nOpcion: ";
        std::cin >> ans2;
        std::cout << std::endl;
      }while(ans2==1);
      corteTmp.dayID = days_count - 1;
      corteTmp.salespersonID = userid;
      corteTmp.numTickets = num_tickets;
      corteTmp.total = corte_total;
      corte.push_back(corteTmp);
    }while(ans2!=2);
    std::cout << "Corte #: " << corte_count << std::endl;
    std::cout << "Cantidad de tickets (ventas): " << num_tickets << std::endl;
    std::cout << "Cantidad de transacciones: " << num_transactions << std::endl;
    std::cout << "Total del corte: $" << corte_total << std::endl;
  }while(ans2!=2);
}

// view sales report
void salesreport()
{
  int ans;
  std::cout << "Reportes de Ventas" << std::endl;
  while(ans!=3)
  {
    std::cout << "\nSeleccionar opcion:" << std::endl;
    std::cout << "1. Todas Las Ventas" << std::endl;
    std::cout << "2. Ventas Por Vendedor" << std::endl;
    std::cout << "3. Salir" << std::endl;
    std::cout << "Opcion: ";
    std::cin >> ans;
    std::cout << std::endl;
    switch(ans){
      case 1: allsales();
        break;
      case 2: viewsalesperemployee();
        break;
      case 3:
        break;
      default:
        break;
    }
  }
}

void viewsalesperemployee()
{
  int userid;
  std::cout << "Escribe # de Usuario" << std::endl;
  std::cout << "___________________" << std::endl;
  for (int i = 0; i < user.size(); i++){
    std::cout << i+1;
    std::cout << ":" << "\t\t" << user[i].name << std::endl;
  }
  std::cout << std::endl << "Usuario: ";
  std::cin >> userid;
  std::cout << std::endl;
  userid--;
  
  struct ssort temp;
  int total = 0, ans;
  for(int i = 0; i < transaction.size(); i++)
  {
    if(user[userid].name==user[transaction[i].salespersonID].name)
    {
      temp.day = days[corte[ticket[transaction[i].ticketID].corteID].dayID].day;
      temp.month = days[corte[ticket[transaction[i].ticketID].corteID].dayID].month;
      temp.year = days[corte[ticket[transaction[i].ticketID].corteID].dayID].year;
      temp.corteID = ticket[transaction[i].ticketID].corteID;
      temp.ticketID = transaction[i].ticketID;
      temp.quantity = transaction[i].quantity;
      temp.total = transaction[i].total;
      temp.description = transaction[i].description;
      ssort.push_back(temp);
    }
  }
  do
  {
    std::cout << std::endl;
    std::cout << std::endl << "Fecha    \tCorte#\tTicket#\t\tCant  \tImporte \tProducto   " << std::endl;
    std::cout << "_______________________________________________________________" << std::endl;
    for(int i = 0; i < ssort.size(); i++)
    {
      std::cout << ssort[i].day << "-" << ssort[i].month << "-" << std::left << std::setw(6) << ssort[i].year << "\t\t" << std::internal << ssort[i].corteID << "\t\t#" << ssort[i].ticketID << "\t\t " << ssort[i].quantity << "\t\t$" <<  ssort[i].total << "  \t  " <<  ssort[i].description << std::endl;
      total++;
    }
    std::cout << std::endl;
    std::cout << "Total de transacciones: " << total << std::endl;
    std::cout << std::endl;
    std::cout << "Mostrar listado en orden: " << std::endl;
    std::cout << "1. Nombre de Producto" << "\t\t A - Z" << std::endl;
    std::cout << "2. Nombre de Producto" << "\t\t Z - A" << std::endl;
    std::cout << "3. Fecha " << "\t\t\t\t Antiguo a Reciente" << std::endl;
    std::cout << "4. Fecha " << "\t\t\t\t Reciente a Antiguo" << std::endl;
    std::cout << "5. Salir" << std::endl;
    std::cout << "Opcion: ";
    std::cin >> ans;
    if(ans!=5)
    {
      if(ans == 3) ans = 5;
      if(ans == 4) ans = 6;
      sortreport(ssort,ans);
      if(ans== 5) ans = 3;
    }
  }while(ans!=5);
  ssort.clear();
}


// all sales
void allsales()
{
  struct ssort temp;
  int total = 0, ans;
  for(int i = 0; i < transaction.size(); i++)
  {
    temp.day = days[corte[ticket[transaction[i].ticketID].corteID].dayID].day;
    temp.month = days[corte[ticket[transaction[i].ticketID].corteID].dayID].month;
    temp.year = days[corte[ticket[transaction[i].ticketID].corteID].dayID].year;
    temp.corteID = ticket[transaction[i].ticketID].corteID;
    temp.ticketID = transaction[i].ticketID;
    temp.quantity = transaction[i].quantity;
    temp.total = transaction[i].total;
    temp.description = transaction[i].description;
    temp.name = user[transaction[i].salespersonID].name;
    ssort.push_back(temp);
  }
  do
  {
    std::cout << std::endl << "Fecha    \tCorte#\tTicket#\t\tCant  \tImporte \tProducto \tVendedor   " << std::endl;
    std::cout << "_______________________________________________________________________________________" << std::endl;
    for(int i = 0; i < ssort.size(); i++)
    {
      std::cout << ssort[i].day << "-" << ssort[i].month << "-" << std::left << std::setw(6) << ssort[i].year << std::internal << "\t\t" << ssort[i].corteID << "\t\t#" << ssort[i].ticketID << "\t\t " << ssort[i].quantity << "\t\t$" << std::left << std::setw(8) << ssort[i].total << std::left << std::setw(15)  << ssort[i].description << std::setw(12) << std::left << ssort[i].name << std::endl;
      total++;
    }
    std::cout << std::endl;
    std::cout << "Total de transacciones: " << total << std::endl;
    std::cout << std::endl;
    std::cout << "Mostrar listado en orden: " << std::endl;
    std::cout << "1. Nombre de Producto " << "\t A - Z" << std::endl;
    std::cout << "2. Nombre de Producto " << "\t Z - A" << std::endl;
    std::cout << "3. Vendedor " << "\t\t\t\t A - Z" << std::endl;
    std::cout << "4. Vendedor " << "\t\t\t\t Z - A" << std::endl;
    std::cout << "5. Fecha " << "\t\t\t\t Antiguo a Reciente" << std::endl;
    std::cout << "6. Fecha " << "\t\t\t\t Reciente a Antiguo" << std::endl;
    std::cout << "7. Salir" << std::endl;
    std::cout << "Opcion: ";
    std::cin >> ans;
    if(ans!=7) sortreport(ssort,ans);
  }while(ans!=7);
  ssort.clear();
}

// view sales per employee
void viewsales(int userid)
{
  struct ssort temp;
  int total = 0, ans;
  for(int i = 0; i < transaction.size(); i++)
  {
    if(user[userid].name==user[transaction[i].salespersonID].name)
    {
      temp.day = days[corte[ticket[transaction[i].ticketID].corteID].dayID].day;
      temp.month = days[corte[ticket[transaction[i].ticketID].corteID].dayID].month;
      temp.year = days[corte[ticket[transaction[i].ticketID].corteID].dayID].year;
      temp.corteID = ticket[transaction[i].ticketID].corteID;
      temp.ticketID = transaction[i].ticketID;
      temp.quantity = transaction[i].quantity;
      temp.total = transaction[i].total;
      temp.description = transaction[i].description;
      ssort.push_back(temp);
    }
  }
  do
  {
    std::cout << std::endl << "Fecha    \tCorte#\tTicket#\t\tCant  \tImporte \tProducto   " << std::endl;
    std::cout << "_______________________________________________________________" << std::endl;
    for(int i = 0; i < ssort.size(); i++)
    {
      std::cout << ssort[i].day << "-" << ssort[i].month << "-" << std::setw(6) << ssort[i].year << "\t\t" << ssort[i].corteID << "\t\t#" << ssort[i].ticketID << "\t\t " << ssort[i].quantity << "\t\t$" <<  ssort[i].total << "  \t  " <<  ssort[i].description << std::endl;
      total++;
    }
    std::cout << std::endl;
    std::cout << "Total de transacciones: " << total << std::endl;
    std::cout << std::endl;
    std::cout << "Mostrar listado en orden: " << std::endl;
    std::cout << "1. Nombre de Producto" << "\t\t A - Z" << std::endl;
    std::cout << "2. Nombre de Producto" << "\t\t Z - A" << std::endl;
    std::cout << "3. Fecha " << "\t\t\t\t Antiguo a Reciente" << std::endl;
    std::cout << "4. Fecha " << "\t\t\t\t Reciente a Antiguo" << std::endl;
    std::cout << "5. Salir" << std::endl;
    std::cout << "Opcion: ";
    std::cin >> ans;
    if(ans!=5)
    {
      if(ans == 3) ans = 5;
      if(ans == 4) ans = 6;
      sortreport(ssort,ans);
      if(ans== 5) ans = 3;
    }
  }while(ans!=5);
  ssort.clear();
}

// sort list of items by name descending and ascending and by stock from lowest to highest
void sortreport(std::vector<struct ssort> list, int type)
{
  bool swapped = true;
  int j = 0;
  struct ssort temporary;
  switch(type)
  {
    case 1:
    {
      while (swapped)
      {
        swapped = false;
        j++;
        for (int i = 0; i < list.size() - j; i++)
        {
          if (list[i].description > list[i + 1].description)
          {
            temporary = list[i];
            list[i] = list[i + 1];
            list[i + 1] = temporary;
            swapped = true;
          }
        }
      }
    }
      break;
    case 2:
    {
      while (swapped)
      {
        swapped = false;
        j++;
        for (int i = 0; i < list.size() - j; i++)
        {
          if (list[i].description < list[i + 1].description)
          {
            temporary = list[i];
            list[i] = list[i + 1];
            list[i + 1] = temporary;
            swapped = true;
          }
        }
      }
    }
      break;
    case 3:
    {
      while (swapped)
      {
        swapped = false;
        j++;
        for (int i = 0; i < list.size() - j; i++)
        {
          if (list[i].name > list[i + 1].name)
          {
            temporary = list[i];
            list[i] = list[i + 1];
            list[i + 1] = temporary;
            swapped = true;
          }
        }
      }
    }
      break;
    case 4:
    {
      while (swapped)
      {
        swapped = false;
        j++;
        for (int i = 0; i < list.size() - j; i++)
        {
          if (list[i].name < list[i + 1].name)
          {
            temporary = list[i];
            list[i] = list[i + 1];
            list[i + 1] = temporary;
            swapped = true;
          }
        }
      }
    }
      break;
    case 5:
    {
      while (swapped)
      {
        swapped = false;
        j++;
        for (int i = 0; i < list.size() - j; i++)
        {
          if (list[i].year >= list[i + 1].year)
          {
            if(list[i].month > list[i+1].month)
            {
              temporary = list[i];
              list[i] = list[i + 1];
              list[i + 1] = temporary;
              swapped = true;
            }
            if(list[i].month == list[i+1].month)
            {
              if(list[i].day > list[i+1].day)
              {
                temporary = list[i];
                list[i] = list[i + 1];
                list[i + 1] = temporary;
                swapped = true;
              }
            }
          }
        }
      }
    }
      break;
    case 6:
    {
      while (swapped)
      {
        swapped = false;
        j++;
        for (int i = 0; i < list.size() - j; i++)
        {
          if (list[i].year <= list[i + 1].year)
          {
            if(list[i].month < list[i+1].month)
            {
              temporary = list[i];
              list[i] = list[i + 1];
              list[i + 1] = temporary;
              swapped = true;
            }
            if(list[i].month == list[i+1].month)
            {
              if(list[i].day < list[i+1].day)
              {
                temporary = list[i];
                list[i] = list[i + 1];
                list[i + 1] = temporary;
                swapped = true;
              }
            }
          }
        }

      }
    }
      break;
    default:
      break;
  }
  std::vector<struct ssort> *itemPointer;
  itemPointer = &ssort;
  *itemPointer = list;
}

// sort list of items by name descending and ascending and by stock from lowest to highest
void sort(std::vector<struct item> list, int type)
{
  bool swapped = true;
  int j = 0;
  struct item temporary;
  switch(type)
  {
    case 1:
    {
      while (swapped)
      {
        swapped = false;
        j++;
        for (int i = 0; i < list.size() - j; i++)
        {
          if (list[i].name > list[i + 1].name)
          {
            temporary = list[i];
            list[i] = list[i + 1];
            list[i + 1] = temporary;
            swapped = true;
          }
        }
      }
    }
      break;
    case 2:
    {
      while (swapped)
      {
        swapped = false;
        j++;
        for (int i = 0; i < list.size() - j; i++)
        {
          if (list[i].name < list[i + 1].name)
          {
            temporary = list[i];
            list[i] = list[i + 1];
            list[i + 1] = temporary;
            swapped = true;
          }
        }
      }
    }
      break;
    case 3:
    {
      while (swapped)
      {
        swapped = false;
        j++;
        for (int i = 0; i < list.size() - j; i++)
        {
          if (list[i].stock > list[i + 1].stock)
          {
            temporary = list[i];
            list[i] = list[i + 1];
            list[i + 1] = temporary;
            swapped = true;
          }
        }
      }
    }
      break;
    default:
      break;
  }
  std::vector<struct item> *itemPointer;
  itemPointer = &vsort;
  *itemPointer = list;
}

// search for item in inventory and return 0.1 for not found, x < 0 for inactive, x == 0 || x > 0.1 for active
double search(struct item search){
  double found = 0.1;
  for(int x = 0; x < item.size(); x++)
  {
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

// hidden admin option, set all inventory items to random between 0 and 30
void settoRand()
{
  int ans, password;
  std::cout << "Ingresar contrasena de administrador: ";
  std::cin >> password;
  if(password ==  1234)
  {
    std::cout << "Confirmar que quiere cambiar todos los articulos a existencias entre 0 y 30 al azar. (1 = Confirmar, 2 = Cancelar): ";
    std::cin >> ans;
    if(ans == 1) for(int i = 0; i < item.size(); i++) item[i].stock = rand() % 30 + 0;
    else std::cout << "Cancelado";
    std::cout << std::endl;
  }
}

// save item list to CSV file
void outputcsv()
{
  std::ofstream csvfile;
  csvfile.open ("itemlist.csv");
  csvfile << "Producto" << "," << "$Compra" << "," << "$Venta" << "," << "Existencias" << "," << "Stock Min" << "," << "Inactive" << std::endl;
  for(int i = 0; i < item.size(); i++)
  {
    csvfile << item[i].name << "," << item[i].purchaseprice << "," << item[i].sellingprice << "," << item[i].stock << "," << item[i].minstock << "," << item[i].inactive;
    if(item[i].stock <= item[i].minstock) csvfile << ",*";
    csvfile << std::endl;
  }
  csvfile.close();
  std::cout << "Archivo CSV creado. " << std::endl << std::endl;
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
  outfile.open("users.bin", std::ios::binary);
  for(int i = 0; i < user.size(); i++)
  {
    outfile.write(reinterpret_cast<char *> (&user[i].name),sizeof(user[i].name));
    outfile.write(reinterpret_cast<char *> (&user[i].password),sizeof(user[i].password));
  }
  outfile.close();
  outfile.open("transactions.bin", std::ios::binary);
  for(int i = 0; i < transaction.size(); i++)
  {
    outfile.write(reinterpret_cast<char *> (&transaction[i].ticketID),sizeof(transaction[i].ticketID));
    outfile.write(reinterpret_cast<char *> (&transaction[i].salespersonID),sizeof(transaction[i].salespersonID));
    outfile.write(reinterpret_cast<char *> (&transaction[i].description),sizeof(transaction[i].description));
    outfile.write(reinterpret_cast<char *> (&transaction[i].quantity),sizeof(transaction[i].quantity));
    outfile.write(reinterpret_cast<char *> (&transaction[i].price),sizeof(transaction[i].price));
    outfile.write(reinterpret_cast<char *> (&transaction[i].total),sizeof(transaction[i].total));
  }
  outfile.close();
  outfile.open("tickets.bin", std::ios::binary);
  for(int i = 0; i < ticket.size(); i++)
  {
    outfile.write(reinterpret_cast<char *> (&ticket[i].corteID),sizeof(ticket[i].corteID));
    outfile.write(reinterpret_cast<char *> (&ticket[i].numTransactions),sizeof(ticket[i].numTransactions));
    outfile.write(reinterpret_cast<char *> (&ticket[i].total),sizeof(ticket[i].total));
  }
  outfile.close();
  outfile.open("cortes.bin", std::ios::binary);
  for(int i = 0; i < corte.size(); i++)
  {
    outfile.write(reinterpret_cast<char *> (&corte[i].dayID),sizeof(corte[i].dayID));
    outfile.write(reinterpret_cast<char *> (&corte[i].numTickets),sizeof(corte[i].numTickets));
    outfile.write(reinterpret_cast<char *> (&corte[i].salespersonID),sizeof(corte[i].salespersonID));
    outfile.write(reinterpret_cast<char *> (&corte[i].total),sizeof(corte[i].total));
  }
  outfile.close();
  outfile.open("days.bin", std::ios::binary);
  for(int i = 0; i < days.size(); i++)
  {
    outfile.write(reinterpret_cast<char *> (&days[i].day),sizeof(days[i].day));
    outfile.write(reinterpret_cast<char *> (&days[i].month),sizeof(days[i].month));
    outfile.write(reinterpret_cast<char *> (&days[i].year),sizeof(days[i].year));
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
    //std::cout << "Archivo inventory.bin cargado exitosamente." << std::endl;
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
  struct user usuario;
  infile.open("users.bin", std::ios::binary);
  if(infile.fail())
  {
    infile.close();
    outfile.open("users.bin", std::ios::binary);
    outfile.close();
    std::cout << "Archivo users.bin creado. Favor de reiniciar programa." << std::endl;
    exit (EXIT_FAILURE);
  }
  if(!infile.fail())
  {
    // std::cout << "Archivo users.bin cargado exitosamente." << std::endl;
  }
  if (infile.peek() != std::ifstream::traits_type::eof())
  {
    while(!infile.eof())
    {
      infile.read(reinterpret_cast<char *> (&usuario.name),sizeof(usuario.name));
      infile.read(reinterpret_cast<char *> (&usuario.password),sizeof(usuario.password));
      user.push_back(usuario);
    }
    user.pop_back();
  }
  infile.close();
  struct transaction trans;
  infile.open("transactions.bin", std::ios::binary);
  if(infile.fail())
  {
    infile.close();
    outfile.open("transactions.bin", std::ios::binary);
    outfile.close();
    std::cout << "Archivo transactions.bin creado. Favor de reiniciar programa." << std::endl;
    exit (EXIT_FAILURE);
  }
  if(!infile.fail())
  {
    //std::cout << "Archivo transactions.bin cargado exitosamente." << std::endl;
  }
  if (infile.peek() != std::ifstream::traits_type::eof())
  {
    while(!infile.eof())
    {
      infile.read(reinterpret_cast<char *> (&trans.ticketID),sizeof(trans.ticketID));
      infile.read(reinterpret_cast<char *> (&trans.salespersonID),sizeof(trans.salespersonID));
      infile.read(reinterpret_cast<char *> (&trans.description),sizeof(trans.description));
      infile.read(reinterpret_cast<char *> (&trans.quantity),sizeof(trans.quantity));
      infile.read(reinterpret_cast<char *> (&trans.price),sizeof(trans.price));
      infile.read(reinterpret_cast<char *> (&trans.total),sizeof(trans.total));
      transaction.push_back(trans);
    }
    transaction.pop_back();
  }
  infile.close();
  struct ticket tick;
  infile.open("tickets.bin", std::ios::binary);
  if(infile.fail())
  {
    infile.close();
    outfile.open("tickets.bin", std::ios::binary);
    outfile.close();
    std::cout << "Archivo tickets.bin creado. Favor de reiniciar programa." << std::endl;
    exit (EXIT_FAILURE);
  }
  if(!infile.fail())
  {
    //std::cout << "Archivo tickets.bin cargado exitosamente." << std::endl;
  }
  if (infile.peek() != std::ifstream::traits_type::eof())
  {
    while(!infile.eof())
    {
      infile.read(reinterpret_cast<char *> (&tick.corteID),sizeof(tick.corteID));
      infile.read(reinterpret_cast<char *> (&tick.numTransactions),sizeof(tick.numTransactions));
      infile.read(reinterpret_cast<char *> (&tick.total),sizeof(tick.total));
      ticket.push_back(tick);
    }
    ticket.pop_back();
  }
  infile.close();
  struct corte cut;
  infile.open("cortes.bin", std::ios::binary);
  if(infile.fail())
  {
    infile.close();
    outfile.open("cortes.bin", std::ios::binary);
    outfile.close();
    std::cout << "Archivo cortes.bin creado. Favor de reiniciar programa." << std::endl;
    exit (EXIT_FAILURE);
  }
  if(!infile.fail())
  {
    //std::cout << "Archivo cortes.bin cargado exitosamente." << std::endl;
  }
  if (infile.peek() != std::ifstream::traits_type::eof())
  {
    while(!infile.eof())
    {
      infile.read(reinterpret_cast<char *> (&cut.dayID),sizeof(cut.dayID));
      infile.read(reinterpret_cast<char *> (&cut.numTickets),sizeof(cut.numTickets));
      infile.read(reinterpret_cast<char *> (&cut.salespersonID),sizeof(cut.salespersonID));
      infile.read(reinterpret_cast<char *> (&cut.total),sizeof(cut.total));
      corte.push_back(cut);
    }
    corte.pop_back();
  }
  infile.close();
  struct day dia;
  infile.open("days.bin", std::ios::binary);
  if(infile.fail())
  {
    infile.close();
    outfile.open("days.bin", std::ios::binary);
    outfile.close();
    std::cout << "Archivo days.bin creado. Favor de reiniciar programa." << std::endl;
    exit (EXIT_FAILURE);
  }
  if(!infile.fail())
  {
    //std::cout << "Archivo days.bin cargado exitosamente." << std::endl << std::endl;
  }
  if (infile.peek() != std::ifstream::traits_type::eof())
  {
    while(!infile.eof())
    {
      infile.read(reinterpret_cast<char *> (&dia.day),sizeof(dia.day));
      infile.read(reinterpret_cast<char *> (&dia.month),sizeof(dia.month));
      infile.read(reinterpret_cast<char *> (&dia.year),sizeof(dia.year));
      days.push_back(dia);
    }
    days.pop_back();
  }
  infile.close();
}
