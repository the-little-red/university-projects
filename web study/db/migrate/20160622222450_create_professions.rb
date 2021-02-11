class CreateProfessions < ActiveRecord::Migration[5.1]
  def change
    create_table :professions do |t|
      t.string :name
      t.float :salary
      t.references :user, index: true, foreign_key: true

      t.timestamps null: false
    end
  end
end
