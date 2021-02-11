class CreateAddresses < ActiveRecord::Migration[5.1]
  def change
    create_table :addresses do |t|
      t.string :address_house
      t.string :city
      t.string :country

      t.timestamps null: false
    end
  end
end
