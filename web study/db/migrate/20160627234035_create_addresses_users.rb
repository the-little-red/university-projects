class CreateAddressesUsers < ActiveRecord::Migration[5.1]
  def change
    create_table :addresses_users do |t|
      t.references :user, index: true, foreign_key: true
      t.references :address, index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
